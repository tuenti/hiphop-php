<?php
///////////////////////////////////////////////////////////////////////////////
// Preamble: C++ code inserted at beginning of ext_{name}.h

DefinePreamble(<<<CPP

CPP
);

///////////////////////////////////////////////////////////////////////////////
// Constants
//
// array (
//   'name' => name of the constant
//   'type' => type of the constant
//   'note' => additional note about this constant's schema
// )


DefineConstant(array('name' => "BUCKET_INFO_CLUSTER",       'type' => Int64));
DefineConstant(array('name' => "BUCKET_INFO_ID",            'type' => Int64));
DefineConstant(array('name' => "BUCKET_INFO_STATE",         'type' => Int64));
DefineConstant(array('name' => "BUCKET_INFO_SERVER",        'type' => Int64));
DefineConstant(array('name' => "BUCKET_INFO_VOLUME",        'type' => Int64));
DefineConstant(array('name' => "BUCKET_INFO_ALT_SERVER",    'type' => Int64));
DefineConstant(array('name' => "BUCKET_INFO_ALT_VOLUME",    'type' => Int64));

///////////////////////////////////////////////////////////////////////////////
// Functions
//
// array (
//   'name'   => name of the function
//   'desc'   => description of the function's purpose
//   'flags'  => attributes of the function, see base.php for possible values
//   'opt'    => optimization callback function's name for compiler
//   'note'   => additional note about this function's schema
//   'return' =>
//      array (
//        'type'  => return type, use Reference for ref return
//        'desc'  => description of the return value
//      )
//   'args'   => arguments
//      array (
//        'name'  => name of the argument
//        'type'  => type of the argument, use Reference for output parameter
//        'value' => default value of the argument
//        'desc'  => description of the argument
//      )
// )

DefineFunction(
  array(
    'name'   => "get_bucket_cache_info",
    'flags'  =>  HasDocComment,
    'return' => array(
      'type'   => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "cluster_id",
        'type'   => Int64,
      ),
      array(
        'name'   => "bucket_id",
        'type'   => Int64,
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "set_bucket_cache_info",
    'flags'  =>  HasDocComment,
    'return' => array(
      'type'   => Boolean,
    ),
    'args'   => array(
      array(
        'name'   => "cluster_id",
        'type'   => Int64,
      ),
      array(
        'name'   => "bucket_id",
        'type'   => Int64,
      ),
      array(
        'name'   => "state",
        'type'   => Int64,
      ),
      array(
        'name'   => "server",
        'type'   => Int64,
      ),
      array(
        'name'   => "volume",
        'type'   => Int64,
      ),
      array(
        'name'   => "alt_server",
        'type'   => Int64,
      ),
      array(
        'name'   => "alt_volume",
        'type'   => Int64,
      ),
    ),
  ));
