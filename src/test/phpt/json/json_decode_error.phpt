--TEST--
Test json_decode() function : error conditions
--SKIPIF--
<?php
if (!extension_loaded("json")) {
 	die('skip JSON extension not available in this build');
}	 
?>
--FILE--
<?php
/* Prototype  : mixed json_decode  ( string $json  [, bool $assoc=false  [, int $depth=512  ]] )
 * Description: Decodes a JSON string
 * Source code: ext/json/php_json.c
 * Alias to functions: 
 */
echo "*** Testing json_decode() : error conditions ***\n";

echo "\n-- Testing json_decode() function with no arguments --\n";
var_dump( json_decode() );

echo "\n-- Testing json_decode() function with more than expected no. of arguments --\n";
$extra_arg = 10;
var_dump( json_decode('"abc"', TRUE, 512, 0, $extra_arg) );

?>
===Done===
--EXPECTF--
*** Testing json_decode() : error conditions ***

-- Testing json_decode() function with no arguments --
HipHop Warning:  Missing argument 1 for json_decode() in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 10
NULL

-- Testing json_decode() function with more than expected no. of arguments --
HipHop Warning:  Too many arguments for json_decode(), expected 3 in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 14
NULL
===Done===
