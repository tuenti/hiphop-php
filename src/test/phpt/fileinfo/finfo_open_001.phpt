--TEST--
finfo_open(): Testing magic_file names
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

var_dump(finfo_open(FILEINFO_MIME, "\0"));
var_dump(finfo_open(FILEINFO_MIME, NULL));
var_dump(finfo_open(FILEINFO_MIME, ''));
var_dump(finfo_open(FILEINFO_MIME, 123));
var_dump(finfo_open(FILEINFO_MIME, 1.0));
var_dump(finfo_open(FILEINFO_MIME, '/foo/bar/inexistent'));

?>
--EXPECTF--
HipHop Warning:  expects parameter 2 to be a valid path, string given in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 3
bool(false)
resource(5) of type (file_info)
resource(5) of type (file_info)
HipHop Warning:  Error calling magic_load: could not find any magic files! in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 6
bool(false)
HipHop Warning:  Error calling magic_load: could not find any magic files! in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 7
bool(false)
HipHop Warning:  Error calling magic_load: could not find any magic files! in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 8
bool(false)
