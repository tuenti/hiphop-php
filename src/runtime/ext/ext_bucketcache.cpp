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
#include <runtime/ext/ext_bucketcache.h>
#include <runtime/base/runtime_option.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <system/lib/systemlib.h>

 /** 
  * Configuration & helpers
  *
  * The block info is stored as:
  *
  * ---------------------------------------------------------------------------------
  * | Current location               | Alt location                   | Timestamp   |
  * |--------------------------------------------------------------------------------
  * |  state | volume |    server    | unused | volume |    server    |  ts TTL     |
  * | 1 byte | 1 byte |   2 bytes    | 1 byte | 1 byte |   2 bytes    |  4 bytes    |
  * ---------------------------------------------------------------------------------
  */

typedef struct {
  unsigned char state;
  unsigned char volume;
  unsigned short server;
  unsigned char flags;
  unsigned char alt_volume;
  unsigned short alt_server;
  unsigned int expiration;
} bucket_cache_info;

// Uncomment for debugging
//#define BUCKETCACHE_DEBUG 1

#define MAX_CLUSTER_ID 4
#define MAX_BUCKET_ID 524288
#define FILESIZE MAX_BUCKET_ID*MAX_CLUSTER_ID*sizeof(bucket_cache_info) // Enough for 2 Mi bucket infos 1E6 * 3 int * 4 bytes per int ~= 24Mb

#define BUCKET_INFO_CLUSTER 0
#define BUCKET_INFO_ID 1
#define BUCKET_INFO_STATE 2
#define BUCKET_INFO_SERVER 3
#define BUCKET_INFO_VOLUME 4
#define BUCKET_INFO_ALT_SERVER 5
#define BUCKET_INFO_ALT_VOLUME 6

/**
 * Facilitators
 */
#define BUCKET_CACHE                        s_bucketcache_extension.map
#define BUCKET_INITIALIZED                  s_bucketcache_extension.init
#define BUCKET_REQUEST_TIME                 s_bucketcache_extension.request_time
#define BUCKET_CACHE_FILE 					s_bucketcache_extension.cache_file
#define BUCKET_TTL 						    s_bucketcache_extension.ttl
#define BUCKET_STALE_DATA_REUSABILITY_TTL 	s_bucketcache_extension.stale_data_reusability_ttl
#define BUCKET_TTL_FOR_PRIMING_STALE_DATA 	s_bucketcache_extension.ttl_for_priming_stale_data
#define BUCKET_TTL_FOR_PRIMING_STALE_DATA 	s_bucketcache_extension.ttl_for_priming_stale_data
#define BUCKET_MUTEX                     	s_bucketcache_extension.s_mutex

#define GET_CACHE_POS(cluster_id,bucket_id) (cluster_id*MAX_BUCKET_ID)+bucket_id

static inline void debug_printf( const char* format, ... ) { 
#ifdef BUCKETCACHE_DEBUG
  va_list args;
  va_start( args, format );
  vprintf(format, args );
  va_end( args );
#endif
}

namespace HPHP {

const int64 k_BUCKET_INFO_CLUSTER     = BUCKET_INFO_CLUSTER;
const int64 k_BUCKET_INFO_ID          = BUCKET_INFO_ID;
const int64 k_BUCKET_INFO_STATE       = BUCKET_INFO_STATE;
const int64 k_BUCKET_INFO_SERVER      = BUCKET_INFO_SERVER;
const int64 k_BUCKET_INFO_VOLUME      = BUCKET_INFO_VOLUME;
const int64 k_BUCKET_INFO_ALT_SERVER  = BUCKET_INFO_ALT_SERVER;
const int64 k_BUCKET_INFO_ALT_VOLUME  = BUCKET_INFO_ALT_VOLUME;

class bucketcacheExtension: public Extension {
    public:

    bucketcacheExtension() : Extension("bucketcache") { }

    /**
     * Module initialization function
     * - Open / create the cache file
     * - mmap it into a global variable
     */
    virtual void moduleInit()
    {
        init = false;
        request_time = time(NULL);
        cache_file = RuntimeOption::BucketCacheFile.c_str();
        ttl = RuntimeOption::BucketCacheTTL;
        stale_data_reusability_ttl = RuntimeOption::BucketCacheStaleDataReusability;
        ttl_for_priming_stale_data = RuntimeOption::BucketCacheTTLForPrimingStaleData;

        debug_printf("Cachefile: %s, ttl %d, stale_data_reusability_ttl: %d, ttl_for_priming_stale_data: %d\n",
                cache_file, ttl, stale_data_reusability_ttl, ttl_for_priming_stale_data);

        fd = open(cache_file, O_RDWR);
        if (fd == -1) {
            // Create the file
            fd = open(cache_file, O_RDWR | O_CREAT, 0666);
            chmod(cache_file, 0666);
            if (fd == -1) {
                return;
            }
            if (lseek(fd, FILESIZE, SEEK_SET) == -1) {
                close(fd);
                return;
            }
            if (write(fd, "", 1) == -1) {
                close(fd);
                return;
            }
        }

        // Mmap the cache file
        map = static_cast<bucket_cache_info *>(mmap(0, FILESIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0));
        if (map == MAP_FAILED) {
            close(fd);
            return;
        }

        init = true;
    } 

    /**
     * Module shutdown function
     * - Mummap the file
     * - Closes the cache file
     */
    virtual void moduleShutdown() {
        if (init) {
            munmap(map, FILESIZE);
            close(fd);
            init = false;
        }
    }

    bucket_cache_info *map;
    int fd;
    bool init;
    time_t request_time;
    Mutex s_mutex;

    // Vars from config file
    const char * cache_file;
    int ttl;
    int stale_data_reusability_ttl;
    int ttl_for_priming_stale_data;
} s_bucketcache_extension;


void print_debug_info(bucket_cache_info info) {
    debug_printf("Expiration: %i\n", info.expiration);
    debug_printf("State: %d\n", info.state);
    debug_printf("flags: %d\n", info.flags);
    debug_printf("Current   storage: Server: %hu Volume %d\n", info.server, info.volume);
    debug_printf("Alternate storage: Server: %hu Volume %d\n", info.alt_server, info.alt_volume);
}


void throwBucketCacheException(const char *message) {
  Object e(SystemLib::AllocBucketCacheExceptionObject(message));
  throw e;
}

///////////////////////////////////////////////////////////////////////////////
/**
 * get_bucket_cache_info(int $cluster_id, int $bucket_id)
 *
 * @param int $bucket_id
 * @returns array with bucket_cache_info if success, boolean False otherwise
 */
Variant f_get_bucket_cache_info(int64 cluster_id, int64 bucket_id) {
    // Time expiration diff, pos in cache
    int tdiff;
    long pos;
    time_t request_time;

    if (!BUCKET_INITIALIZED) {
		throwBucketCacheException("BucketCache extension not initialized");
    }

    debug_printf("**** get_bucket_cache_info(cluster_id = %li, bucket_id = %li)\n", cluster_id, bucket_id);

    // Check bounds for the bucket_id
    if (bucket_id < 0 || bucket_id >= MAX_BUCKET_ID
        || cluster_id < 0 || cluster_id >= MAX_CLUSTER_ID) {
        debug_printf("ERROR: Bucket/Cluster is out of bounds\n");
        return Variant((bool)false);
    }

    // Calculate cache position
    pos = GET_CACHE_POS(cluster_id, bucket_id);

    request_time = time(NULL);

    // Retrieve information
    tdiff = request_time - BUCKET_CACHE[pos].expiration;
    if (tdiff > 0) {
        // Expired!
        if (tdiff < BUCKET_STALE_DATA_REUSABILITY_TTL) {
            // Extend validity. We will return FALSE so current request will reprime,
            // but we don't want all concurrent request to reprime too, so the
            // winner will extend the timestamp TTL so other think the data is
            // valid, and don't try to reprime at the same time.
            BUCKET_CACHE[pos].expiration = request_time + BUCKET_TTL_FOR_PRIMING_STALE_DATA;
            debug_printf("STALE DATA: Data is old but can be still reused.\n");
            debug_printf("Returning FALSE to this request to start a re-priming, \
                while upcomming requests for %li seconds will see the stale data\n", 
                BUCKET_TTL_FOR_PRIMING_STALE_DATA);
        } else {
            debug_printf("EXPIRED DATA: Data is old and can't be reused. Returning FALSE\n");
        }
        // Not valid
        return Variant((bool)false);
    } else {
        debug_printf("VALID DATA! Returning:\n");
    }

    // Prepare result
    bucket_cache_info info;
    BUCKET_MUTEX.lock();
    info = BUCKET_CACHE[pos];
    BUCKET_MUTEX.unlock();

    Array ret(Array::Create());
    ret.append(cluster_id);
    ret.append(bucket_id);
    ret.append(info.state);
    ret.append(info.server);
    ret.append(info.volume);
    ret.append(info.alt_server);
    ret.append(info.alt_volume);

    print_debug_info(info);

    return ret;
}

/**
 * set_bucket_cache_info(int $cluster_id, int $bucket_id, int $state, int $server, int $volume, int $alt_server, int $alt_volume)
 *
 * @param int $cluster_id
 * @param int $bucket_id
 * @param int $state
 * @param int $server
 * @param int $volume
 * @param int $alt_server
 * @param int $alt_volume
 * @returns boolean True for success, False otherwise
 */
bool f_set_bucket_cache_info(int64 cluster_id, int64 bucket_id, int64 state, int64 server, int64 volume, int64 alt_server, int64 alt_volume) {
    bucket_cache_info info;
    time_t request_time;

    if (!BUCKET_INITIALIZED) {
		throwBucketCacheException("BucketCache extension not initialized");
    }

    debug_printf("**** set_bucket_cache_info(cluster_id, bucket_id = %li, state = %li, server = %li, volume = %li, alt_server = %li, alt_volume = %li)\n",
            bucket_id,
            state,
            server,
            volume,
            alt_server,
            alt_volume
          );

    // Check bounds for the bucket_id
    if (bucket_id < 0 
        || bucket_id >= MAX_BUCKET_ID
        || cluster_id < 0 
        || cluster_id >= MAX_CLUSTER_ID) {
        debug_printf("ERROR: Bucket/Cluster is out of bounds\n");
        return false;
    }

    // Calculate cache position
    long pos = GET_CACHE_POS(cluster_id, bucket_id);

    // Ensure the data fits into it's space
    if (state >> sizeof(info.state)*8 != 0
        || server >> sizeof(info.server)*8 != 0
        || alt_server >> sizeof(info.alt_server)*8 != 0
        || volume >> sizeof(info.volume)*8 != 0
        || alt_volume >> sizeof(info.alt_volume)*8 != 0) {
        debug_printf("ERROR: Provided data is out of bounds\n");
        return false;
    }

    request_time = time(NULL);

    // Store values into the cache:
    info.state = state;
    info.flags = 0;
    info.server = server;
    info.volume = volume;
    info.alt_server = alt_server;
    info.alt_volume = alt_volume;
    info.expiration = request_time + BUCKET_TTL;
    BUCKET_MUTEX.lock();
    BUCKET_CACHE[pos] = info;
    BUCKET_MUTEX.unlock();

    debug_printf("SUCESSFULLY STORED! Actual contents in cache:\n");
    print_debug_info(info);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
}
