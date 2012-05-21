/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#include <test/test_ext_geoip.h>
#include <runtime/ext/ext_geoip.h>

IMPLEMENT_SEP_EXTENSION_TEST(Geoip);
///////////////////////////////////////////////////////////////////////////////

bool TestExtGeoip::RunTests(const std::string &which) {
  bool ret = true;

  RUN_TEST(test_geoip_db_avail);
  RUN_TEST(test_geoip_db_filename);
  RUN_TEST(test_geoip_db_get_all_info);
  RUN_TEST(test_geoip_database_info);
  RUN_TEST(test_geoip_country_code_by_name);
  RUN_TEST(test_geoip_country_name_by_name);
  RUN_TEST(test_geoip_continent_code_by_name);
  RUN_TEST(test_geoip_org_by_name);
  RUN_TEST(test_geoip_record_by_name);
  RUN_TEST(test_geoip_id_by_name);
  RUN_TEST(test_geoip_region_by_name);
  RUN_TEST(test_geoip_isp_by_name);
  RUN_TEST(test_geoip_region_name_by_code);
  RUN_TEST(test_geoip_time_zone_by_country_and_region);

  return ret;
}

///////////////////////////////////////////////////////////////////////////////

bool TestExtGeoip::test_geoip_db_avail() {
  return Count(true);
}

bool TestExtGeoip::test_geoip_db_filename() {
  return Count(true);
}

bool TestExtGeoip::test_geoip_db_get_all_info() {
  return Count(true);
}

bool TestExtGeoip::test_geoip_database_info() {
  return Count(true);
}

bool TestExtGeoip::test_geoip_country_code_by_name() {
  return Count(true);
}

bool TestExtGeoip::test_geoip_country_name_by_name() {
  return Count(true);
}

bool TestExtGeoip::test_geoip_continent_code_by_name() {
  return Count(true);
}

bool TestExtGeoip::test_geoip_org_by_name() {
  return Count(true);
}

bool TestExtGeoip::test_geoip_record_by_name() {
  return Count(true);
}

bool TestExtGeoip::test_geoip_id_by_name() {
  return Count(true);
}

bool TestExtGeoip::test_geoip_region_by_name() {
  return Count(true);
}

bool TestExtGeoip::test_geoip_isp_by_name() {
  return Count(true);
}

bool TestExtGeoip::test_geoip_region_name_by_code() {
  return Count(true);
}

bool TestExtGeoip::test_geoip_time_zone_by_country_and_region() {
  return Count(true);
}
