#!/bin/bash

function abort {
	echo $*
	exit
}

BAD_CACHE="/aaaa/user_cache"

rm -f /tmp/custom.userpartition.cache
rm -f /tmp/custom.bucketcache.bin

if ! $HPHPI -c server.hdf user_partition.php ; then 
	abort "Error executing tests"
fi

if ! $HPHPI -c server.hdf bucket_cache.php ; then 
	abort "Error executing tests"
fi

if $HPHPI -v UserPartition.CacheFile=$BAD_CACHE -c server.hdf user_partition.php ; then 
	abort "Error: this test is suppose to throw an exception and return 255"
fi

if $HPHPI -v BucketCache.CacheFile=$BAD_CACHE -c server.hdf bucket_cache.php ; then 
	abort "Error: this test is suppose to throw an exception and return 255"
fi

rm -f /tmp/custom.userpartition.cache
rm -f /tmp/custom.bucketcache.bin

echo "All test runs OK (exceptions thrown in the lasts ones are OK)"
