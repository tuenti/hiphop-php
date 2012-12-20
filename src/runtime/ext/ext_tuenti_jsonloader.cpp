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
#include <runtime/ext/ext_json.h>
#include <runtime/ext/ext_file.h>
#include <runtime/ext/ext_apc.h>
#include <runtime/base/shared/shared_store.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <runtime/ext/ext_tuenti_jsonloader.h>

namespace HPHP {
IMPLEMENT_DEFAULT_EXTENSION(tuenti_jsonloader);

//#define JSONCACHE_DEBUG

// We use this store as is the only way I found to store a Variant in a way
// that is request-persistant. This will be accessed by multiple threads so we
// need the lock-protected version
RwLockHashTableSharedStore s_json_store(1);

static inline void debug_printf( const char* format, ... ) {
#ifdef JSONCACHE_DEBUG
    va_list args;
    va_start( args, format );
    vprintf(format, args );
    va_end( args );
#endif
}

Variant f_cached_json_decode(CStrRef json_file, bool assoc /* = false */,
                      bool loose /* = false */) {
  if (json_file.empty()) {
    return null;
  }

  int json_refresh_time = RuntimeOption::JsonCacheRefreshTime;

  debug_printf("Checking file %s, assoc %i loose %i\n", json_file.data(), assoc, loose);

  struct stat json_stat;
  time_t now = time(NULL);
  int stat_err;

  if (s_json_store.exists(json_file)) {
    Variant entry;
    s_json_store.get(json_file, entry);
    time_t lastupdate = (int) entry[LAST_UPDATE];
    time_t lastupdate_nsecs = (int) entry[LAST_UPDATE_NSECS];
    time_t lastcheck = (int) entry[LAST_CHECK];

    debug_printf("Cache entry found, lastcheck=%ld lastupdate=%ld:%ld, now=%ld\n", lastcheck, lastupdate, lastupdate_nsecs, now);

    if (now < lastcheck + json_refresh_time) {
      debug_printf("Cache entry found and fresh\n");
      return entry;    
    }
    
    // We hit refresh timeout, checking file modification time
    stat_err = stat(json_file.data(), &json_stat);
 
    if (!stat_err && (json_stat.st_mtime != lastupdate || json_stat.st_mtim.tv_nsec != lastupdate_nsecs)) {
      debug_printf("Cache entry found, entry is still valid, but need to update lastcheck to %ld\n", now);
      // No need to reparse
      Array newentry;
      newentry.append(entry[JSON]);
      newentry.append(json_stat.st_mtime);
      newentry.append(json_stat.st_mtim.tv_nsec);
      newentry.append(now);
      s_json_store.store(json_file, newentry, 0, true);
      return entry;
    }
  } else {
    stat_err = stat(json_file.data(), &json_stat);
  }

  Array newentry;
  if (stat_err) {
    debug_printf("File not found\n");
    newentry.append(null);
    newentry.append(0);
    newentry.append(0);
    newentry.append(now);
  } else {
    String file_contents = f_file_get_contents(json_file);
    Variant decoded_json = file_contents ?
      f_json_decode(file_contents, assoc, loose) : null;
    newentry.append(decoded_json);
    newentry.append(json_stat.st_mtime);
    newentry.append(json_stat.st_mtim.tv_nsec);
    newentry.append(now);
  }

  debug_printf("Cache entry not found or old, decoding and storing it\n");

  s_json_store.store(json_file, newentry, 0, true);

  return newentry;
}

}
