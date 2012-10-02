/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
   | Copyright (c) 1997-2010 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#include <runtime/ext/ext_gettext.h>
#include <runtime/ext/ext_file.h>
#include <libintl.h>
#include <string.h>
#include <boost/locale.hpp>
#include <util/lock.h>
#include <util/logger.h>
#include <iostream>
#include <stdexcept>

namespace HPHP {
IMPLEMENT_DEFAULT_EXTENSION(gettext);

class GettextData {
public:

  GettextData() {
      // Select std backend as default
      boost::locale::localization_backend_manager std_be = boost::locale::localization_backend_manager::global();
      std_be.select("std");
      boost::locale::localization_backend_manager::global(std_be);
      gen = new boost::locale::generator(std_be);

      gen->locale_cache_enabled(true);
      // Storing default locale. 
      // Is has to be done here because gettext is not thread safe
      default_locale = gen->generate("");
      std::locale::global(default_locale);
      default_locale_name = setlocale(LC_ALL, NULL);
      default_domain = textdomain(NULL);
      default_path = bindtextdomain(default_domain.c_str(), NULL);
  }

  std::locale default_locale;
  std::string default_domain;
  std::string default_path;
  std::string default_locale_name;

  hphp_hash_map<std::string, std::locale> locale_map;
  ReadWriteMutex locale_mutex;
  boost::locale::generator *gen;
};

static GettextData s_gettext;

class GettextRequestData : public RequestEventHandler {
public:
  virtual void requestInit() {
      if (!RuntimeOption::GettextDefaultLocale.empty()) {
        m_locale_name = RuntimeOption::GettextDefaultLocale; 
        m_locale = s_gettext.gen->generate(m_locale_name);
      } else {
        m_locale = s_gettext.default_locale;
        m_locale_name = s_gettext.default_locale_name; 
      }
      m_domain = s_gettext.default_domain;
      m_domains_map[m_domain] = s_gettext.default_path;
  }

  virtual void requestShutdown() {
      m_domains_map.clear();
  }

  std::locale get_locale() {
      return m_locale;
  }

  void set_locale(std::string locale) {
      m_locale = s_gettext.gen->generate(locale);
      m_locale_name = locale;
      if (RuntimeOption::GettextDebug) {
        Logger::Verbose("[Gettext] Changing default locale to %s", locale.c_str());
      }
  }

  std::locale & get_cached_locale(std::string domain) {
      if (m_domains_map.count(domain) == 0)
          m_domains_map[domain] = s_gettext.default_path;

      std::string key = m_locale_name + "#" + domain + "#" + m_domains_map[domain];

      bool found;
      {
        ReadLock lock(s_gettext.locale_mutex);
        found = (s_gettext.locale_map.count(key.c_str()) > 0);
      }

      if (!found) {
        if (RuntimeOption::GettextDebug) {
          Logger::Verbose("[Gettext] Creating locale for %s", key.c_str());
        }

        boost::locale::gnu_gettext::messages_info info;
        info.paths.push_back(m_domains_map[domain]);
        info.domains.push_back(boost::locale::gnu_gettext::messages_info::domain(domain));

        // Use boost::locale::info to configure all parameters
        boost::locale::info const &properties = std::use_facet<boost::locale::info>(m_locale);
        info.language = properties.language();
        info.country  = properties.country();
        info.encoding = properties.encoding();
        info.variant  = properties.variant();

        try {
          std::locale new_locale(m_locale, boost::locale::gnu_gettext::create_messages_facet<char>(info));
          WriteLock lock(s_gettext.locale_mutex);
          // Check that no other thread creates the entry in the meantime
          if (s_gettext.locale_map.count(key) == 0) {
            s_gettext.locale_map[key] = new_locale;
          }
        } catch (std::runtime_error e) {
          raise_warning("Gettext: Locale not found, falling back to default locale: %s", m_locale_name.c_str());
          WriteLock lock(s_gettext.locale_mutex);
          // Check that no other thread creates the entry in the meantime
          if (s_gettext.locale_map.count(key) == 0) {
            s_gettext.locale_map[key] = m_locale;
          }
        }
      } 

      ReadLock lock(s_gettext.locale_mutex);
      std::locale & l = s_gettext.locale_map[key.c_str()];
      return l;
  }

    // Default locale and domain of the request
    std::locale m_locale;
    std::string m_locale_name;
    std::string m_domain;

    // Map of domain => locale path
    std::map<std::string, std::string> m_domains_map;
};

IMPLEMENT_STATIC_REQUEST_LOCAL(GettextRequestData, s_gettext_request);

/*
 * This function is being called from f_setlocale. That way we can
 * make code compatible with PHP behaviour
 */
void set_request_locale(CStrRef locale) {
    s_gettext_request->set_locale(locale.c_str());
}

std::locale get_request_locale() {
    return s_gettext_request->get_locale();
}

Variant f_textdomain(CStrRef domain) {
    if (!domain.empty()) {
      s_gettext_request->m_domain = domain.c_str();
    }

    return domain;
}

Variant f_gettext(CStrRef msgid) {
    return f_dgettext(s_gettext_request->m_domain, msgid);
}

Variant f__(CStrRef msgid) {
    return f_dgettext(s_gettext_request->m_domain, msgid);
}

Variant f_bindtextdomain(CStrRef domain, CStrRef dir) {
    if (domain.empty()) {
        raise_warning("The first parameter of bindtextdomain must not be empty");
        return false;
    }

    Variant final_path;
    if (!dir.empty() && dir != "0") {
        final_path = f_realpath(dir);
    } else {
        final_path = f_getcwd();
    }
    if (!final_path) {
        return false;
    }

    s_gettext_request->m_domains_map[domain.c_str()] = final_path.toString().c_str();
    return final_path;
}

Variant f_dgettext(CStrRef domain_name, CStrRef msgid) {
    std::locale & l = s_gettext_request->get_cached_locale(domain_name.c_str());
    return boost::locale::dgettext(domain_name.c_str(), msgid.c_str(), l);
}

Variant f_dcgettext(CStrRef domain_name, CStrRef msgid, int64 category) {
    throw NotImplementedException(__func__);
}

Variant f_ngettext(CStrRef msg, CStrRef msg_plural, int64 n) {
    return f_dngettext(s_gettext_request->m_domain, msg.c_str(), msg_plural.c_str(), n);
}

Variant f_dngettext(CStrRef domain, CStrRef msg, CStrRef msg_plural, int64 n) {
    std::locale & l = s_gettext_request->get_cached_locale(domain.c_str());
    return boost::locale::dngettext(domain.c_str(), msg.c_str(), msg_plural.c_str(), n, l);
}

Variant f_dcngettext(CStrRef domain, CStrRef msg, CStrRef msg_plural, int64 n, int64 category) {
    throw NotImplementedException(__func__);
}

Variant f_bind_textdomain_codeset(CStrRef domain, CStrRef codeset) {
    // TODO implement getting the locale from the cache and changing it
    throw NotImplementedException(__func__);
}

int locale_strcoll(CStrRef str1, CStrRef str2) {
    boost::locale::comparator<char> comp(get_request_locale(), boost::locale::collator_base::secondary);
    return comp(str1.c_str(), str2.c_str());
}

}
