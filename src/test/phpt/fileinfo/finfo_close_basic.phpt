--TEST--
Test finfo_close() function : basic functionality 
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php
/* Prototype  : resource finfo_close(resource finfo)
 * Description: Close fileinfo resource. 
 * Source code: ext/fileinfo/fileinfo.c
 * Alias to functions: 
 */

echo "*** Testing finfo_close() : basic functionality ***\n";

$magicFile = dirname( __FILE__ ) . DIRECTORY_SEPARATOR . 'fileinfo/magic.mgc';

$finfo = finfo_open( FILEINFO_MIME, $magicFile );
var_dump( $finfo );

// Calling finfo_close() with all possible arguments
var_dump( finfo_close($finfo) );

?>
===DONE===
--EXPECTF--
*** Testing finfo_close() : basic functionality ***
resource(4) of type (file_info)
bool(true)
===DONE===
