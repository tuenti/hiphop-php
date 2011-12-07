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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <runtime/ext/ext_userpartition.h>
#include <runtime/base/runtime_option.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <system/lib/systemlib.h>
#include <stdexcept>

namespace HPHP {

/**
 * Configuration
 */
#define BITS_PER_PARTITION 6
#define PARTITIONS_PER_BLOCK (int)floor(sizeof(int)*8/BITS_PER_PARTITION)
#define PARTITION_MASK (1<<BITS_PER_PARTITION) - 1
#define MIN_PARTITION 1
#define MAX_PARTITION 52
// Uncomment for debugging
//#define USERPARTITION_DEBUG

/**
 * Valid users are 0..100K and 59.4M ... MAX_USER_ID+59.4M
 */
#define MAX_USER_ID 100000000
#define GAP_START 100000
#define GAP_END 59400000

/**
 * Cache file
 */
#define DEFAULT_FILEPATH "/tmp/mmapped.bin"
#define FILESIZE MAX_USER_ID*sizeof(int)/PARTITIONS_PER_BLOCK // Enough for 100 M partitions 100E6 / 5 per int * 32 bits per int / 8 bits per byte = 80Mb

/**
 * Facilitators
 */
#define USER_PARTITION_MAP s_userpartition_extension.map
#define USER_PARTITION_FILE s_userpartition_extension.fd
#define USER_PARTITION_INIT s_userpartition_extension.init

#define BLOCK_NUMBER(i) (int)floor((i-1)/PARTITIONS_PER_BLOCK)
#define DISPLACEMENT_IN_BLOCK(i) (int)(i-1)%PARTITIONS_PER_BLOCK*BITS_PER_PARTITION

static inline void debug_printf( const char* format, ... ) { 
#ifdef USERPARTITION_DEBUG
    va_list args;
    va_start( args, format );
    vprintf(format, args );
    va_end( args );
#endif
}

void throwUserPartitionException(const char *message) {
  Object e(SystemLib::AllocUserPartitionExceptionObject(message));
  throw e;
}

class userpartitionExtension: public Extension {
 public:
 userpartitionExtension() : Extension("userpartition") { }

 /**
 * Module initialization function
 * - Open / create the cache file
 * - mmap it into a global variable
 */
 virtual void moduleInit()
 {
	 // It would be better to throw an exception here, but seems that is not
	 // possible on the module init, so we throw an exception on the first call
	 // to a function
	 const char * cache_file = RuntimeOption::UserPartitionCacheFile.c_str();
	 init = false;

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
	 map = static_cast<int *>(mmap(0, FILESIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0));
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

  int *map;
  int fd;
  bool init;
} s_userpartition_extension;


/**
 * Adjusts the given partition id (user_id) to remove the
 * gap between GAP_START .. GAP_END
 */
long get_adjusted_partition_id(long i) {
	if (i > 0 && i <= GAP_START) {
		// Below the gap, directly the partition id
		return i;
	} else if (i >= GAP_END && i < (MAX_USER_ID+GAP_END-GAP_START)) {
		// Above the gap, substract GAP_END-GAP_START
		return i-(GAP_END-GAP_START);
	} else {
		// Invalid key,
		return (long) 0;
	}
}

/**
 * get_tuenti_user_partition(int $user_id)
 *
 * @param int $user_id
 * @returns int partition Id if success, boolean False otherwise
 */
Variant f_get_tuenti_user_partition(int64 user_id) {
	long block;

	// Exit if not initialized
	if (! USER_PARTITION_INIT) {
		throwUserPartitionException("UserPartition extension not initialized");
	}

	// Adjust the requested user_id:
	user_id = get_adjusted_partition_id(user_id);
	if (user_id == 0) {
		return Variant((bool)false);
	}

	// Retrieve block and calculate the partition id
	block = USER_PARTITION_MAP[BLOCK_NUMBER(user_id)];
	debug_printf("adj_userid = %lli, block = %i, block content = %li, block content displaces = %li, displacement = %i\n", 
        user_id, BLOCK_NUMBER(user_id), block, block >> DISPLACEMENT_IN_BLOCK(user_id), DISPLACEMENT_IN_BLOCK(user_id));

	block >>= DISPLACEMENT_IN_BLOCK(user_id);
	debug_printf("resulting partition %li\n", block & PARTITION_MASK);

	block &= PARTITION_MASK;

	if (block == 0) {
		return Variant((bool) false);
	} else {
		return Variant((int64) block);
	}
}

/**
 * set_tuenti_user_partition(int $user_id, int $partition_id)
 *
 * @param int $user_id
 * @param int $partition_id
 * @returns boolean True for success, False otherwise
 */
bool f_set_tuenti_user_partition(int64 user_id, int64 partition_id) {
	long block;

	// Exit if not initialized
	if (! USER_PARTITION_INIT) {
		throwUserPartitionException("UserPartition extension not initialized");
	}

	// Ensure the partition is between margins
	if (partition_id < MIN_PARTITION || partition_id > MAX_PARTITION) {
		return false;
	}

	// Adjust the requested user_id:
	user_id = get_adjusted_partition_id(user_id);
	if (user_id == 0) {
		return false;
	}

	block = USER_PARTITION_MAP[BLOCK_NUMBER(user_id)];

	// Prepare the block to write
	block = partition_id;
	block <<= DISPLACEMENT_IN_BLOCK(user_id);

	// Log the operation
	debug_printf("adj_userid = %lli, block = %i, block content = %li, block content displaces = %li, displacement = %i \n", 
        user_id, BLOCK_NUMBER(user_id), block, block >> DISPLACEMENT_IN_BLOCK(user_id), DISPLACEMENT_IN_BLOCK(user_id));

	// Atomically, OR it with in the partition map
	__sync_or_and_fetch(&USER_PARTITION_MAP[BLOCK_NUMBER(user_id)], block);

	return true;
}

}
