--TEST--
Test finfo_close() function : error conditions 
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php
/* Prototype  : resource finfo_close(resource finfo)
 * Description: Close fileinfo resource. 
 * Source code: ext/fileinfo/fileinfo.c
 * Alias to functions: 
 */

echo "*** Testing finfo_close() : error conditions ***\n";

$magicFile = dirname( __FILE__ ) . DIRECTORY_SEPARATOR . 'fileinfo/magic.mgc';
$finfo = finfo_open( FILEINFO_MIME, $magicFile );
$fp = fopen( __FILE__, 'r' );

echo "\n-- Testing finfo_close() function with Zero arguments --\n";
var_dump( finfo_close() );

echo "\n-- Testing finfo_close() function with more than expected no. of arguments --\n";
var_dump( finfo_close( $finfo, '10') );

echo "\n-- Testing finfo_close() function with wrong resource type --\n";
var_dump( finfo_close( $fp ) );

?>
===DONE===
--EXPECTF--
*** Testing finfo_close() : error conditions ***

-- Testing finfo_close() function with Zero arguments --
HipHop Warning:  Missing argument 1 for finfo_close() in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 15
NULL

-- Testing finfo_close() function with more than expected no. of arguments --
HipHop Warning:  Too many arguments for finfo_close(), expected 1 in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 18
NULL

-- Testing finfo_close() function with wrong resource type --
HipHop Fatal error: Unexpected object type PlainFile. in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 21
