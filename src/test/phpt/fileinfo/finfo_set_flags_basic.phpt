--TEST--
Test finfo_set_flags() function : basic functionality 
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php
/* Prototype  : bool finfo_set_flags(resource finfo, int options)
 * Description: Set libmagic configuration options. 
 * Source code: ext/fileinfo/fileinfo.c
 * Alias to functions: 
 */

$magicFile = dirname(__FILE__) . DIRECTORY_SEPARATOR . 'fileinfo/magic.mgc';
$finfo = finfo_open( FILEINFO_MIME, $magicFile );

echo "*** Testing finfo_set_flags() : basic functionality ***\n";

var_dump( finfo_set_flags( $finfo, FILEINFO_NONE ) );
var_dump( finfo_set_flags( $finfo, FILEINFO_SYMLINK ) );
var_dump( finfo_set_flags() );

finfo_close( $finfo );

?>
===DONE===
--EXPECTF--
*** Testing finfo_set_flags() : basic functionality ***
bool(true)
bool(true)
HipHop Warning:  Missing argument 1 for finfo_set_flags() in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 15
NULL
===DONE===
