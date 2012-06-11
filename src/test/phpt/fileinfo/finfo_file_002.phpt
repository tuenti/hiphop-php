--TEST--
finfo_file(): Testing mime types
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$fp = finfo_open(FILEINFO_MIME_TYPE);
$results = array();

foreach (glob(dirname(__FILE__) . DIRECTORY_SEPARATOR . "fileinfo/resources/*") as $filename) {
	if (is_file($filename)) {
		$results[basename($filename)] = finfo_file($fp, $filename);
	}
}
ksort($results);

var_dump($results);
?>
--EXPECTF--
array(6) {
  ["dir.zip"]=>
  string(15) "application/zip"
  ["test.bmp"]=>
  string(14) "image/x-ms-bmp"
  ["test.gif"]=>
  string(9) "image/gif"
  ["test.jpg"]=>
  string(10) "image/jpeg"
  ["test.pdf"]=>
  string(15) "application/pdf"
  ["test.png"]=>
  string(9) "image/png"
}
