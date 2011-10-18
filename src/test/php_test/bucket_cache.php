<?php

// Some helper functions

function error($msg, &$errors) {
    $errors[] = $msg;
    //print $msg . "\n";
    //exit(1);
}

function set_bucket_and_store_error($cluster_id, $bucket_id, $state, $server, $volume, $alt_server, $alt_volume, &$errors) {
    if (set_bucket_cache_info($cluster_id, $bucket_id, $state, $server, $volume, $alt_server, $alt_volume) !== TRUE) {
        error("FALSE obtained when storing ($cluster_id, $bucket_id, $state, $server, $volume, $alt_server, $alt_volume)", $errors);
    }
}

function check_bucket_cache_values($cluster_id, $bucket_id, $state, $server, $volume, $alt_server, $alt_volume, &$errors, $expected = TRUE) {
    $bi = get_bucket_cache_info($cluster_id, $bucket_id);

    if ($expected === FALSE) {
        if ($bi !== FALSE) {
            error("Expected False on get_bucket_cache_info but retreived data", $errros);
        }
        return;
    }

    if ($bi === FALSE) {
	    error("Returned value not the expected when recovering previously stored bucket $bucket_id on cluster $cluster_id", $errors);
        return;
    }

    if (($bi[BUCKET_INFO_CLUSTER]       !== $cluster_id)
        || ($bi[BUCKET_INFO_ID]         !== $bucket_id)
        || ($bi[BUCKET_INFO_STATE]      !== $state)
        || ($bi[BUCKET_INFO_SERVER]     !== $server)
        || ($bi[BUCKET_INFO_VOLUME]     !== $volume)
        || ($bi[BUCKET_INFO_ALT_SERVER] !== $alt_server)
        || ($bi[BUCKET_INFO_ALT_VOLUME] !== $alt_volume)) {
        $e = "Values retreived are not the expected";
        $e .= "\nExpected:  ($cluster_id, $bucket_id, $state, $server, $volume, $alt_server, $alt_volume)";
        $e .= "\nRetreived: (" . implode(", ", $bi) . ")";
        error($e, $errors);
    }
}

function testRange($start, $end, &$errors) {
	for ($i=$start; $i<$end; $i++) {
		set_bucket_and_store_error($i % 4, $i, $i%20, $i%21, $i%22, $i%23, $i%24, $errors);
	}

	for ($i=$start; $i<$end; $i++) {
        check_bucket_cache_values($i % 4, $i, $i%20,  $i%21, $i%22, $i%23, $i%24, $errors);
	}
}

$start = microtime(true);

$errors = array();
if (!function_exists('get_bucket_cache_info')) {
	error('Function does not exist. Module not loaded?', $errors);
}

if (get_bucket_cache_info(1, 1) !== FALSE) {
	error('FALSE was expected when requesting an uncached partition', $errors);
}

// Basic testing
set_bucket_and_store_error(3, 1, 2, 3, 4, 5, 6, $errors);
set_bucket_and_store_error(3, 2, 2, 3, 4, 5, 6, $errors);
set_bucket_and_store_error(3, 1, 2, 3, 4, 5, 6, $errors);
check_bucket_cache_values(3, 1, 2, 3, 4, 5, 6, $errors);
check_bucket_cache_values(3, 2, 2, 3, 4, 5, 6, $errors);
check_bucket_cache_values(3, 1, 2, 3, 4, 5, 6, $errors);

// Smoke tests
if (set_bucket_cache_info(-1, 1, 2, 3, 4, 5, 6) !== FALSE) {
		error("FALSE was expected when setting a invalid cluster_id -1", $errors);
}

if (set_bucket_cache_info(10, 1, 2, 3, 4, 5, 6) !== FALSE) {
		error("FALSE was expected when setting a invalid cluster_id 10", $errors);
}

if (set_bucket_cache_info(4, 524289, 2, 3, 4, 5, 6) !== FALSE) {
		error("FALSE was expected when setting a invalid bucket_id 524289", $errors);
}

if (set_bucket_cache_info(4, -1, 2, 3, 4, 5, 6) !== FALSE) {
		error("FALSE was expected when setting a invalid bucket_id -1", $errors);
}

// Testing with some load
testRange(30, 500000, $errors);

// TTL testing
$ttl = 10;
$stale = 5;
$extra_ttl = 7;

set_bucket_and_store_error(1, 1, 2, 3, 4, 5, 6, $errors);

// Should be ok
check_bucket_cache_values(1, 1, 2, 3, 4, 5, 6, $errors);
sleep(2);
check_bucket_cache_values(1, 1, 2, 3, 4, 5, 6, $errors);

// Sleep ttl seconds and check for expiration (only first request)
sleep($ttl);
check_bucket_cache_values(1, 1, 2, 3, 4, 5, 6, $errors, FALSE);

// Now we expect TRUE (due to stale data reusing)
check_bucket_cache_values(1, 1, 2, 3, 4, 5, 6, $errors);
sleep(1);
check_bucket_cache_values(1, 1, 2, 3, 4, 5, 6, $errors);

// Sleep for the extra ttl added on the reuse and check for false (but only for first request)
sleep($ttl);
check_bucket_cache_values(1, 1, 2, 3, 4, 5, 6, $errors, FALSE);
check_bucket_cache_values(1, 1, 2, 3, 4, 5, 6, $errors);

// Now we sleep so much that data is not good even for been reuse and check for false
sleep($extra_ttl + $stale + 1);
check_bucket_cache_values(1, 1, 2, 3, 4, 5, 6, $errors, FALSE);
check_bucket_cache_values(1, 1, 2, 3, 4, 5, 6, $errors, FALSE);
check_bucket_cache_values(1, 1, 2, 3, 4, 5, 6, $errors, FALSE);

// Same check but with fresh data
set_bucket_and_store_error(1, 1, 2, 3, 4, 5, 6, $errors);
sleep($ttl + $stale + 1);
check_bucket_cache_values(1, 1, 2, 3, 4, 5, 6, $errors, FALSE);
check_bucket_cache_values(1, 1, 2, 3, 4, 5, 6, $errors, FALSE);
check_bucket_cache_values(1, 1, 2, 3, 4, 5, 6, $errors, FALSE);

if (sizeof($errors) > 0) {
	echo "*** ERRORS ***\n";
	die(implode("\n", $errors)."\n");
} else {
	$cache_file = "/tmp/bucketcache.bin";
	$stat = stat($cache_file);
	echo "*** All test run fine! ***\n";
    printf("Cache file size for 0.5M consecutive buckets: %0.2f Mb / %0.2f Mb\n", $stat['blksize']*$stat['blocks']/1024/1024, $stat['size']/1024/1024);
	printf("Running time: %0.2fs (includes 0.5M get / sets)\n", microtime(true)-$start);
}


