--TEST--
Test finfo_open() function : error functionality 
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php
/* Prototype  : resource finfo_open([int options [, string arg]])
 * Description: Create a new fileinfo resource. 
 * Source code: ext/fileinfo/fileinfo.c
 * Alias to functions: 
 */

$magicFile = dirname(__FILE__) . DIRECTORY_SEPARATOR . 'fileinfo/magic.mgc';

echo "*** Testing finfo_open() : error functionality ***\n";

var_dump( finfo_open( FILEINFO_MIME, 'foobarfile' ) );
var_dump( finfo_open( FILEINFO_MIME, $magicFile, 'extraArg' ) );
var_dump( finfo_open( PHP_INT_MAX - 1, $magicFile ) );

?>
===DONE===
--EXPECTF--
*** Testing finfo_open() : error functionality ***
HipHop Warning:  Error calling magic_load: could not find any magic files! in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 12
bool(false)
HipHop Warning:  Too many arguments for finfo_open(), expected 2 in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 13
NULL
resource(5) of type (file_info)
===DONE===
