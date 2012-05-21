--TEST--
Calling geoip_database_info() with a non-existant database type within bound.
--SKIPIF--
<?php if (!extension_loaded("geoip")) print "skip"; ?>
--FILE--
<?php

geoip_database_info(14);

?>
--EXPECTF--
HipHop Warning:  Required database not available at /usr/share/GeoIP/GeoIP.dat. in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 3
