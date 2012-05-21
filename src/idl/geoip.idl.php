<?php
/*
 * This extension is just a way to implement some missing functions without making
 * a new extension to implement each one
 *
 */
///////////////////////////////////////////////////////////////////////////////
// Preamble: C++ code inserted at beginning of ext_{name}.h

DefinePreamble(<<<CPP

CPP
);

DefineFunction(
  array(
    'name'   => "geoip_db_avail",
    'flags'  =>  HasDocComment,
    'return' => array(
      'type' => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "database",
        'type'   => Int64,
      ),
     )
  ));

DefineFunction(
  array(
    'name'   => "geoip_db_filename",
    'flags'  =>  HasDocComment,
    'return' => array(
      'type' => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "database",
        'type'   => Int64,
      ),
     )
  ));

DefineFunction(
  array(
    'name'   => "geoip_db_get_all_info",
    'flags'  =>  HasDocComment,
    'return' => array(
      'type' => VariantMap,
    ),
  ));

DefineFunction(
  array(
    'name'   => "geoip_database_info",
    'flags'  =>  HasDocComment,
    'desc'   => "Returns GeoIP Database information",
    'return' => array(
      'type' => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "database",
        'type'   => Int64,
        'value'  => "k_GEOIP_COUNTRY_EDITION",
      ),
     )
  ));

DefineFunction(
  array(
    'name'   => "geoip_country_code_by_name",
    'flags'  =>  HasDocComment,
    'desc'   => "Return the Country Code found in the GeoIP Database",
    'return' => array(
      'type' => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "hostname",
        'type'   => String,
      ),
     )
  ));

DefineFunction(
  array(
    'name'   => "geoip_country_code3_by_name",
    'flags'  =>  HasDocComment,
    'desc'   => "Return the Country Code found in the GeoIP Database",
    'return' => array(
      'type' => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "hostname",
        'type'   => String,
      ),
     )
  ));

DefineFunction(
  array(
    'name'   => "geoip_country_name_by_name",
    'flags'  =>  HasDocComment,
    'desc'   => "Returns the Country name found in the GeoIP Database",
    'return' => array(
      'type' => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "hostname",
        'type'   => String,
      ),
     )
  ));

DefineFunction(
  array(
    'name'   => "geoip_continent_code_by_name",
    'flags'  =>  HasDocComment,
    'desc'   => "Returns the Continent name found in the GeoIP Database",
    'return' => array(
      'type' => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "hostname",
        'type'   => String,
      ),
     )
  ));

DefineFunction(
  array(
    'name'   => "geoip_org_by_name",
    'flags'  =>  HasDocComment,
    'desc'   => "Returns the Organization Name found in the GeoIP Database",
    'return' => array(
      'type' => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "hostname",
        'type'   => String,
      ),
     )
  ));

DefineFunction(
  array(
    'name'   => "geoip_record_by_name",
    'flags'  =>  HasDocComment,
    'desc'   => "Returns the detailed City information found in the GeoIP Database",
    'return' => array(
      'type' => VariantMap,
    ),
    'args'   => array(
      array(
        'name'   => "hostname",
        'type'   => String,
      ),
     )
  ));

DefineFunction(
  array(
    'name'   => "geoip_id_by_name",
    'flags'  =>  HasDocComment,
    'desc'   => "Returns the Net Speed found in the GeoIP Database",
    'return' => array(
      'type' => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "hostname",
        'type'   => String,
      ),
     )
  ));

DefineFunction(
  array(
    'name'   => "geoip_region_by_name",
    'flags'  =>  HasDocComment,
    'desc'   => "Returns the Country Code and region found in the GeoIP Database",
    'return' => array(
      'type' => VariantMap,
    ),
    'args'   => array(
      array(
        'name'   => "hostname",
        'type'   => String,
      ),
     )
  ));

DefineFunction(
  array(
    'name'   => "geoip_isp_by_name",
    'flags'  =>  HasDocComment,
    'desc'   => "Returns the ISP Name found in the GeoIP Database",
    'return' => array(
      'type' => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "hostname",
        'type'   => String,
      ),
     )
  ));

DefineFunction(
  array(
    'name'   => "geoip_region_name_by_code",
    'flags'  =>  HasDocComment,
    'desc'   => "Returns the region name for some country code and region code combo",
    'return' => array(
      'type' => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "country_code",
        'type'   => String,
      ),
      array(
        'name'   => "region_code",
        'type'   => String,
      ),
     )
  ));

DefineFunction(
  array(
    'name'   => "geoip_time_zone_by_country_and_region",
    'flags'  =>  HasDocComment,
    'desc'   => "Returns the time zone for some country code and region code combo",
    'return' => array(
      'type' => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "country_code",
        'type'   => String,
      ),
      array(
        'name'   => "region_code",
        'type'   => String,
        'value'  => "null_string",
      ),
     )
  ));

/* For database type constants */
DefineConstant(
  array(
    'name' => "GEOIP_COUNTRY_EDITION",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_REGION_EDITION_REV0",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_CITY_EDITION_REV0",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_ORG_EDITION",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_ISP_EDITION",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_CITY_EDITION_REV1",
    'type'  => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_REGION_EDITION_REV1",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_PROXY_EDITION",
    'type' => Int64,
  )); 

DefineConstant(
  array(
    'name' => "GEOIP_ASNUM_EDITION",
    'type' => Int64,
  ));   

DefineConstant(
  array(
    'name' => "GEOIP_NETSPEED_EDITION",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_DOMAIN_EDITION",
    'type' => Int64,
  ));

/* For netspeed constants */
DefineConstant(
  array(
    'name' => "GEOIP_UNKNOWN_SPEED",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_DIALUP_SPEED",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_CABLEDSL_SPEED",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_CORPORATE_SPEED",
    'type' => Int64,
  ));
	

DefineConstant(
  array(
    'name' => "GEOIP_STANDARD",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_MEMORY_CACHE",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_CHECK_CACHE",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_INDEX_CACHE",
    'type' => Int64,
  ));

DefineConstant(
  array(
    'name' => "GEOIP_MMAP_CACHE",
    'type' => Int64,
  ));
