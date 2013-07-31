--TEST--
json_decode() tests
--SKIPIF--
<?php if (!extension_loaded("json")) print "skip"; ?>
--FILE--
<?php

var_dump(json_decode());
var_dump(json_decode(""));
var_dump(json_decode("", 1));
var_dump(json_decode("", 0));
var_dump(json_decode(".", 1));
var_dump(json_decode(".", 0));
var_dump(json_decode("<?>"));
var_dump(json_decode(";"));
var_dump(json_decode("руссиш"));
var_dump(json_decode("blah"));
var_dump(json_decode(NULL));
var_dump(json_decode('{ "test": { "foo": "bar" } }'));
var_dump(json_decode('{ "test": { "foo": "" } }'));
var_dump(json_decode('{ "": { "foo": "" } }'));
var_dump(json_decode('{ "": { "": "" } }'));
var_dump(json_decode('{ "": { "": "" }'));
var_dump(json_decode('{ "": "": "" } }'));

?>
===DONE===
--EXPECTF--
HipHop Warning:  Missing argument 1 for json_decode() in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 3
NULL
NULL
NULL
NULL
NULL
NULL
NULL
NULL
NULL
NULL
NULL
object(stdClass)#1 (1) {
  ["test"]=>
  object(stdClass)#2 (1) {
    ["foo"]=>
    string(3) "bar"
  }
}
object(stdClass)#2 (1) {
  ["test"]=>
  object(stdClass)#3 (1) {
    ["foo"]=>
    string(0) ""
  }
}
object(stdClass)#3 (1) {
  ["_empty_"]=>
  object(stdClass)#4 (1) {
    ["foo"]=>
    string(0) ""
  }
}
object(stdClass)#4 (1) {
  ["_empty_"]=>
  object(stdClass)#5 (1) {
    ["_empty_"]=>
    string(0) ""
  }
}
NULL
NULL
===DONE===
