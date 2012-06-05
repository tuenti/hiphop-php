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

#include <runtime/ext/ext_geoip.h>
#include <sys/stat.h>
#include <util/logger.h>

namespace HPHP {
IMPLEMENT_DEFAULT_EXTENSION(geoip);

class GeoIPRequestData {
public:
  GeoIPRequestData() {
    for (int i=0; i<NUM_DB_TYPES; i++) {
      db_last_modified[i] = db_last_modified[i] = 0;
      gi[i] = NULL;
    }
     
    open_flags = RuntimeOption::GeoIPOpenFlags;

    if (RuntimeOption::GeoIPCustomDirectory.size() > 0) {
      GeoIP_setup_custom_directory((char *)RuntimeOption::GeoIPCustomDirectory.c_str());
    }
  }

  ~GeoIPRequestData() {
    unload();
  }  

  void unload() {
    for (int i=0; i<NUM_DB_TYPES; i++) {
      db_last_modified[i] = db_last_modified[i] = 0;
      if (gi[i] != NULL) {
        GeoIP_delete(gi[i]);
        gi[i] = NULL;
      }
    }
  }

  int open_flags;
  GeoIP * gi[NUM_DB_TYPES];
  time_t  db_last_modified[NUM_DB_TYPES];
  time_t  db_last_checked[NUM_DB_TYPES];
};

static IMPLEMENT_THREAD_LOCAL(GeoIPRequestData, s_geoip);

const int64 k_GEOIP_COUNTRY_EDITION = GEOIP_COUNTRY_EDITION;
const int64 k_GEOIP_REGION_EDITION_REV0 = GEOIP_REGION_EDITION_REV0;
const int64 k_GEOIP_CITY_EDITION_REV0 = GEOIP_CITY_EDITION_REV0;
const int64 k_GEOIP_ORG_EDITION = GEOIP_ORG_EDITION;
const int64 k_GEOIP_ISP_EDITION = GEOIP_ISP_EDITION;
const int64 k_GEOIP_CITY_EDITION_REV1 = GEOIP_CITY_EDITION_REV1;
const int64 k_GEOIP_REGION_EDITION_REV1 = GEOIP_REGION_EDITION_REV1;
const int64 k_GEOIP_PROXY_EDITION = GEOIP_PROXY_EDITION;
const int64 k_GEOIP_ASNUM_EDITION = GEOIP_ASNUM_EDITION;
const int64 k_GEOIP_NETSPEED_EDITION = GEOIP_NETSPEED_EDITION;
const int64 k_GEOIP_DOMAIN_EDITION = GEOIP_DOMAIN_EDITION;
const int64 k_GEOIP_UNKNOWN_SPEED = GEOIP_UNKNOWN_SPEED;
const int64 k_GEOIP_DIALUP_SPEED = GEOIP_DIALUP_SPEED;
const int64 k_GEOIP_CABLEDSL_SPEED = GEOIP_CABLEDSL_SPEED;
const int64 k_GEOIP_CORPORATE_SPEED = GEOIP_CORPORATE_SPEED;
const int64 k_GEOIP_STANDARD = GEOIP_STANDARD;
const int64 k_GEOIP_MEMORY_CACHE = GEOIP_MEMORY_CACHE;
const int64 k_GEOIP_CHECK_CACHE = GEOIP_CHECK_CACHE;
const int64 k_GEOIP_INDEX_CACHE = GEOIP_INDEX_CACHE;
const int64 k_GEOIP_MMAP_CACHE = GEOIP_MMAP_CACHE;

void check_enabled() {
  if (! RuntimeOption::GeoIPEnabled) {
    throw DisabledException("GeoIP");
  }
}

bool is_db_avail(int64 database, bool log_warning = true) {
  check_enabled();

  if (database < 0 || database >= NUM_DB_TYPES)
  {   
    raise_warning("Database type given is out of bound.");
    return false;
  }   

  if (!RuntimeOption::GeoIPAutoreload && s_geoip->gi[database]) {
    return true;
  }

  if (! GeoIP_db_avail(database) && log_warning) {
    raise_warning("Required database not available at %s.", GeoIPDBFileName[database]);
    return false;
  }

  time_t now = time(NULL);

  // Refresh time is 5 seconds, to avoid to stat files all the time
  if (now < s_geoip->db_last_checked[database] + 5) {
    return true;
  }

  // We hit refresh timeout, checking file modification time
  struct stat db_stat;
  stat(GeoIPDBFileName[GEOIP_COUNTRY_EDITION], &db_stat);
  s_geoip->db_last_checked[database] = now;

  if (db_stat.st_mtime <= s_geoip->db_last_modified[database]) {
    return true;
  }

  if (s_geoip->gi[database]) GeoIP_delete(s_geoip->gi[database]);
  s_geoip->gi[database] = GeoIP_open_type(database, s_geoip->open_flags);
  s_geoip->db_last_modified[database] = now;

  Logger::Verbose("Loading %s db",  GeoIPDBFileName[database]);

  if (!s_geoip->gi[database] && log_warning) {
    raise_warning("Required database not available at %s.", GeoIPDBFileName[database]);
  }

  return s_geoip->gi[database] != NULL;
}

///////////////////////////////////////////////////////////////////////////////

Variant f_geoip_db_avail(int64 database) {
  return is_db_avail(database);
}

Variant f_geoip_db_filename(int64 database) {
  if (!is_db_avail(database))
    return NULL;

  if (NULL == GeoIPDBFileName[database])
    return NULL;

  return String(GeoIPDBFileName[database], CopyString);
}

Array f_geoip_db_get_all_info() {
  if (!is_db_avail(GEOIP_COUNTRY_EDITION))
    return NULL;

  Array return_value = Array::Create();

  for (int i=0; i < NUM_DB_TYPES; i++)
  {
    if (NULL != GeoIPDBDescription[i])
    {
      Array tmp = Array::Create();

      tmp.set("available", GeoIP_db_avail(i));
      tmp.set("description", (char *)GeoIPDBDescription[i]);
      tmp.set("filename", GeoIPDBFileName[i]);

      return_value.set(i, tmp);
    }
  }

  return return_value;
}

Variant f_geoip_database_info(int64 database /* = k_GEOIP_COUNTRY_EDITION */) {
  if (!is_db_avail(database))
    return NULL;

  const char *db_info = GeoIP_database_info(s_geoip->gi[database]);

  return String(db_info, AttachString);
}

Variant f_geoip_country_code_by_name(CStrRef hostname) {
  if (!is_db_avail(GEOIP_COUNTRY_EDITION))
    return NULL;

  const char * country_code = GeoIP_country_code_by_name(s_geoip->gi[GEOIP_COUNTRY_EDITION], hostname.data());
  if (country_code == NULL) {
    raise_notice("Host %s not found", hostname.data());
    return false;
  }

  return String(country_code, CopyString);
}

Variant f_geoip_country_code3_by_name(CStrRef hostname) {
  if (!is_db_avail(GEOIP_COUNTRY_EDITION))
    return NULL;

  const char * country_code = GeoIP_country_code3_by_name(s_geoip->gi[GEOIP_COUNTRY_EDITION], hostname.data());
  if (country_code == NULL) {
    raise_notice("Host %s not found", hostname.data());
    return false;
  }

  return String(country_code, CopyString);
}

Variant f_geoip_country_name_by_name(CStrRef hostname) {
  if (!is_db_avail(GEOIP_COUNTRY_EDITION))
    return NULL;

  const char *country_name = GeoIP_country_name_by_name(s_geoip->gi[GEOIP_COUNTRY_EDITION], hostname.data());
  if (country_name == NULL) {
    raise_notice("Host %s not found", hostname.data());
    return false;
  }

  return String(country_name, CopyString);
}

Variant f_geoip_continent_code_by_name(CStrRef hostname) {
  if (!is_db_avail(GEOIP_COUNTRY_EDITION))
    return NULL;

  int id = GeoIP_id_by_name(s_geoip->gi[GEOIP_COUNTRY_EDITION], hostname);
  if (id == 0) {
    raise_notice("Host %s not found", hostname.data());
    return false;
  }

  return GeoIP_country_continent[id];
}

Variant f_geoip_org_by_name(CStrRef hostname) {
  if (!is_db_avail(GEOIP_COUNTRY_EDITION))
    return NULL;

  char *org = GeoIP_org_by_name(s_geoip->gi[GEOIP_COUNTRY_EDITION], hostname);
  if (org == NULL) {
    raise_notice("Host %s not found", hostname.data());
    return false;
  }
  
  return String(org, AttachString);
}

Array f_geoip_record_by_name(CStrRef hostname) {
  GeoIP * gi;

  if (is_db_avail(GEOIP_CITY_EDITION_REV1, false)) {
    gi = s_geoip->gi[GEOIP_CITY_EDITION_REV1];
  } else if (is_db_avail(GEOIP_CITY_EDITION_REV0)) {
    gi = s_geoip->gi[GEOIP_CITY_EDITION_REV0];
  }
  else return NULL;

  Array return_value = Array::Create();

  GeoIPRecord * gir = GeoIP_record_by_name(gi, hostname);

  if (NULL == gir) {
    raise_notice("Host %s not found", hostname.data());
    return return_value;
  }

#if LIBGEOIP_VERSION >= 1004003
  return_value.set("continent_code", (gir->continent_code == NULL) ? "" : gir->continent_code);
#endif
  return_value.set("country_code", (gir->country_code == NULL) ? "" : gir->country_code);
  return_value.set("country_code3", (gir->country_code3 == NULL) ? "" : gir->country_code3);
  return_value.set("country_name", (gir->country_name == NULL) ? "" : gir->country_name);
  return_value.set("region", (gir->region == NULL) ? "" : gir->region);
  return_value.set("city", (gir->city == NULL) ? "" : gir->city);
  return_value.set("postal_code", (gir->postal_code == NULL) ? "" : gir->postal_code);
  return_value.set("latitude", gir->latitude);
  return_value.set("longitude", gir->longitude);
#if LIBGEOIP_VERSION >= 1004005
  return_value.set("dma_code", gir->metro_code);
#else
  return_value.set("dma_code", gir->dma_code);
#endif
  return_value.set("area_code", gir->area_code);

  GeoIPRecord_delete(gir);

  return return_value;
}

Variant f_geoip_id_by_name(CStrRef hostname) {
  if (!is_db_avail(GEOIP_NETSPEED_EDITION))
    return NULL;

  return GeoIP_id_by_name(s_geoip->gi[GEOIP_NETSPEED_EDITION], hostname.data());
}

Array f_geoip_region_by_name(CStrRef hostname) {
  GeoIP * gi;

  if (is_db_avail(GEOIP_REGION_EDITION_REV1, false)) {
    gi = s_geoip->gi[GEOIP_REGION_EDITION_REV1];
  } else if (is_db_avail(GEOIP_REGION_EDITION_REV0)) {
    gi = s_geoip->gi[GEOIP_REGION_EDITION_REV0];
  }
  else return NULL;

  Array return_value = Array::Create();
  GeoIPRegion * region = GeoIP_region_by_name(gi, hostname.data());

  if (NULL == region) {
    raise_notice("Host %s not found", hostname.data());
    return false;
  }

  return_value.set("country_code", region->country_code);
  return_value.set("region", region->region);

  GeoIPRegion_delete(region);

  return return_value;
}

Variant f_geoip_isp_by_name(CStrRef hostname) {
  if (!is_db_avail(GEOIP_ISP_EDITION))
    return NULL;

  char * isp = GeoIP_name_by_name(s_geoip->gi[GEOIP_ISP_EDITION], hostname.data());
  if (isp == NULL) {
    raise_notice("Host %s not found", hostname.data());
    return false;
  }

  return String(isp, AttachString);
}

Variant f_geoip_region_name_by_code(CStrRef country_code, CStrRef region_code) {
  check_enabled();

  if (country_code.empty() || region_code.empty()) {
    raise_warning("You need to specify the country and region codes.");
    return false;
  }

  const char *region_name = GeoIP_region_name_by_code(country_code.data(), region_code.data());
  if (region_name == NULL) {
    return false;
  }

  return String(region_name, CopyString);
}

Variant f_geoip_time_zone_by_country_and_region(CStrRef country_code, CStrRef region_code /* = null */) {
  check_enabled();

  if (country_code.empty()) {
    raise_warning("You need to specify at least the country code.");
    return false;
  }

  const char *timezone = GeoIP_time_zone_by_country_and_region(country_code.data(), region_code.data());
  if (timezone == NULL) {
    return false;
  }

  return String(timezone, CopyString);
}

bool f_geoip_db_reload() {
  s_geoip->unload();
  return is_db_avail(GEOIP_COUNTRY_EDITION, false);
}

///////////////////////////////////////////////////////////////////////////////
}
