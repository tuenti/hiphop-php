<?php

$start = microtime(true);

$errors = array();
if (!function_exists('get_tuenti_user_partition')) {
	$errors[] = 'Function does not exist. Module not loaded?';
}

if (get_tuenti_user_partition(1) !== FALSE) {
	$errors[] = 'FALSE was expected when requesting an uncached partition';
}

if (set_tuenti_user_partition(1, 1) !== TRUE) {
	$errors[] = 'TRUE was expected when setting a new partition';
}

$wrong_partition_ids=array(-1,0,53,100000);
foreach ($wrong_partition_ids as $partitionId) {
	if (set_tuenti_user_partition(1, $partitionId) !== FALSE) {
		$errors[] = 'FALSE was expected when setting a invalid partition ('.$partitionId.')';
	}
}

$wrong_user_ids=array(-1, 150000, 160000000);
foreach ($wrong_user_ids as $userId) {
	if (set_tuenti_user_partition($userId, 1) !== FALSE) {
		$errors[] = 'FALSE was expected when setting a new partition for an invalid user id ('.$userId.')';
	}
	if (get_tuenti_user_partition($userId) !== FALSE) {
		$errors[] = 'FALSE was expected when getting a partition for an invalid user id ('.$userId.')';
	}
}

testRange(2, 100000, $errors);
testRange(60000000, 61000000, $errors);

function testRange($start, $end, &$errors) {
	for ($i=$start; $i<$end; $i++) {
		set_tuenti_user_partition($i, ($i%52)+1);
	}

	for ($i=$start; $i<$end; $i++) {
		$user_partition = get_tuenti_user_partition($i);
		if ($user_partition !== (($i%52)+1)) {
			$errors[] = 'Wrong partition recovered for '.$i.'. Was '.$user_partition.' expected '.(($i%52)+1);
			break;
		}
	}
}

if (sizeof($errors)>0) {
	echo "*** ERRORS ***\n";
	die(implode("\n", $errors)."\n");
} else {
	$cache_file = ini_get('userpartition.cache_file');
	$stat = stat($cache_file);
	echo "*** All test run fine! ***\n";
	printf("Cache file size for 1.1M consecutive users: %0.2f Mb / %0.2f Mb\n", $stat['blksize']*$stat['blocks']/1024/1024, $stat['size']/1024/1024);
	printf("Running time: %0.2fs (includes 1.1M get / sets)\n", microtime(true)-$start);
}
