--TEST--
XMLReader: libxml2 XML Reader, moveToElement 
--SKIPIF--
<?php if (!extension_loaded("xmlreader")) print "skip"; ?>
--FILE--
<?php 
/* $Id: 006.phpt,v 1.1.2.2 2005/12/21 03:58:59 pajoye Exp $ */

$xmlstring = '<?xml version="1.0" encoding="UTF-8"?>
<books><book num="1"></book><test /></books>';

$reader = new XMLReader();

$reader->XML($xmlstring);

// 2 read to get on the 2nd node 
$reader->read();
$reader->read();

if ($reader->nodeType != XMLREADER::END_ELEMENT) {
	if ($reader->nodeType == XMLREADER::ELEMENT && $reader->hasAttributes) {
		$attr = $reader->moveToFirstAttribute();
		if ($reader->moveToElement()) {
			if ($reader->name == 'book') {
				echo "ok\n";
			}
		}
	}
}

$reader->close();
?>
===DONE===
--EXPECT--
ok
===DONE===
