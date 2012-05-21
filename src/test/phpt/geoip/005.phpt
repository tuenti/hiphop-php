--TEST--
Checking for out of bound type with geoip_db_avail()
--SKIPIF--
<?php if (!extension_loaded("geoip")) print "skip"; ?>
--FILE--
<?php

geoip_db_avail(-1000);
geoip_db_avail(1000);

?>
--EXPECTF--
HipHop Warning:  Database type given is out of bound. in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 3
HipHop Warning:  Database type given is out of bound. in /home/daniel/tuenti/hiphop-php/src/test/phpt/file on line 4
