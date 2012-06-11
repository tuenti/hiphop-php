--TEST--
finfo_file(): Testing file names
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$fp = finfo_open();
var_dump(finfo_file($fp, "\0"));
var_dump(finfo_file($fp, ''));
var_dump(finfo_file($fp, NULL));
var_dump(finfo_file($fp, '.'));
var_dump(finfo_file($fp, '&'));

?>
--EXPECTF--
HipHop Warning:  finfo_file: Empty filename or path in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 4
bool(false)
HipHop Warning:  finfo_file: Empty filename or path in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 5
bool(false)
HipHop Warning:  finfo_file: Empty filename or path in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 6
bool(false)
string(9) "directory"
HipHop Warning:  Error calling magic_file: cannot open `&' (No such file or directory) in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 8
bool(false)
