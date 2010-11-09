<?php
/**
 * Automatically generated by running "php schema.php fb".
 *
 * You may modify the file, but re-running schema.php against this file will
 * standardize the format without losing your schema changes. It does lose
 * any changes that are not part of schema. Use "note" field to comment on
 * schema itself, and "note" fields are not used in any code generation but
 * only staying within this file.
 */
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

DefineConstant(
  array(
    'name'   => "FB_UNSERIALIZE_NONSTRING_VALUE",
    'type'   => Int64,
  ));

DefineConstant(
  array(
    'name'   => "FB_UNSERIALIZE_UNEXPECTED_END",
    'type'   => Int64,
  ));

DefineConstant(
  array(
    'name'   => "FB_UNSERIALIZE_UNRECOGNIZED_OBJECT_TYPE",
    'type'   => Int64,
  ));

DefineConstant(
  array(
    'name'   => "FB_UNSERIALIZE_UNEXPECTED_ARRAY_KEY_TYPE",
    'type'   => Int64,
  ));

DefineConstant(
  array(
    'name'   => "XHPROF_FLAGS_NO_BUILTINS",
    'type'   => Int64,
  ));

DefineConstant(
  array(
    'name'   => "XHPROF_FLAGS_CPU",
    'type'   => Int64,
  ));

DefineConstant(
  array(
    'name'   => "XHPROF_FLAGS_MEMORY",
    'type'   => Int64,
  ));

DefineConstant(
  array(
    'name'   => "XHPROF_FLAGS_VTSC",
    'type'   => Int64,
  ));

DefineConstant(
  array(
    'name'   => "XHPROF_FLAGS_TRACE",
    'type'   => Int64,
  ));

DefineConstant(
  array(
    'name'   => "XHPROF_FLAGS_MEASURE_XHPROF_DISABLE",
    'type'   => Int64,
  ));

DefineConstant(
  array(
    'name'   => "XHPROF_FLAGS_MALLOC",
    'type'   => Int64,
  ));


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
    'name'   => "fb_thrift_serialize",
    'desc'   => "Same as fb_serialize(). Please use fb_serialize() with better naming. This function has nothing to do with Facebook's open sourced Thrift package.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Variant,
      'desc'   => "Serialized data.",
    ),
    'args'   => array(
      array(
        'name'   => "thing",
        'type'   => Variant,
        'desc'   => "What to serialize. Note that objects are not supported.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_thrift_unserialize",
    'desc'   => "Same as fb_unserialize(). Please use fb_unserialize() with better naming. This function has nothing to do with Facebook's open sourced Thrift package.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Variant,
      'desc'   => "Unserialized data.",
    ),
    'args'   => array(
      array(
        'name'   => "thing",
        'type'   => Variant,
        'desc'   => "What to unserialize.",
      ),
      array(
        'name'   => "success",
        'type'   => Variant | Reference,
        'desc'   => "Whether it was successful or not.",
      ),
      array(
        'name'   => "errcode",
        'type'   => Variant | Reference,
        'value'  => "null_variant",
        'desc'   => "One of those FB_UNSERIALIZE_ constants to describe what the decoding error was, if it failed.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_serialize",
    'desc'   => "Seriailize data into a compact format that can be unserialized by fb_unserialize().",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Variant,
      'desc'   => "Serialized data.",
    ),
    'args'   => array(
      array(
        'name'   => "thing",
        'type'   => Variant,
        'desc'   => "What to serialize. Note that objects are not supported.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_unserialize",
    'desc'   => "Unserialize a previously fb_thrift_serialize()-ed data.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Variant,
      'desc'   => "Unserialized data.",
    ),
    'args'   => array(
      array(
        'name'   => "thing",
        'type'   => Variant,
        'desc'   => "What to unserialize.",
      ),
      array(
        'name'   => "success",
        'type'   => Variant | Reference,
        'desc'   => "Whether it was successful or not.",
      ),
      array(
        'name'   => "errcode",
        'type'   => Variant | Reference,
        'value'  => "null_variant",
        'desc'   => "One of those FB_UNSERIALIZE_ constants to describe what the decoding error was, if it failed.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_intercept",
    'desc'   => "Invokes a user handler upon calling a function or a class method. If this handler returns FALSE, code will continue with original function. Otherwise, it will return what handler tells. The handler function looks like \"intercept_handler(\$name, \$obj, \$params, \$data, &\$done)\", where \$name is orginal function's name, \$obj is \$this for an instance method call or null for static method call or function calls, and \$params are original call's parameters. \$data is what's passed to fb_intercept() and set \$done to false to indicate function should continue its execution with old function as if interception did not happen. By default \$done is true so it will return handler's return immediately without executing old function's code. Note that built-in functions are not interceptable.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Boolean,
      'desc'   => "TRUE if successful, FALSE otherwise",
    ),
    'args'   => array(
      array(
        'name'   => "name",
        'type'   => String,
        'desc'   => "The function or class method name to intercept. Use \"class::method\" for method name. If empty, all functions will be intercepted by the specified handler and registered individual handlers will be replaced. To make sure individual handlers not affected by such a call, call fb_intercept() with individual names afterwards.",
      ),
      array(
        'name'   => "handler",
        'type'   => Variant,
        'desc'   => "Callback to handle the interception. Use null, false or empty string to unregister a previously registered handler. If name is empty, all previously registered handlers, including those that are set by individual function names, will be removed.",
      ),
      array(
        'name'   => "data",
        'type'   => Variant,
        'value'  => "null_variant",
        'desc'   => "Extra data to pass to the handler when intercepting",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_stubout_intercept_handler",
    'desc'   => "As an interception handler, invokes a different function instead. Useful for stubbing out a function in unit testing. To use this handler, call fb_intercept('old_func', 'fb_stubout_intercept_handler', 'new_func') and this will call new_func() every time when old_func() is called and return new_func()'s return as old_func() call's return.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Variant,
      'desc'   => "What to return on behalf of original function.",
    ),
    'args'   => array(
      array(
        'name'   => "name",
        'type'   => String,
        'desc'   => "Which function function or class method that is being intercepted.",
      ),
      array(
        'name'   => "obj",
        'type'   => Variant,
        'desc'   => "Which class object this method was calling on.",
      ),
      array(
        'name'   => "params",
        'type'   => VariantVec,
        'desc'   => "The parameters original function took.",
      ),
      array(
        'name'   => "data",
        'type'   => Variant,
        'desc'   => "The callback to redirect to.",
      ),
      array(
        'name'   => "done",
        'type'   => Variant | Reference,
        'desc'   => "Will always set to TRUE.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_rpc_intercept_handler",
    'desc'   => "As an interception handler, invokes a function remotely on an RPC server instead of locally.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Variant,
      'desc'   => "What to return on behalf of original function.",
    ),
    'args'   => array(
      array(
        'name'   => "name",
        'type'   => String,
        'desc'   => "Which function function or class method that is being intercepted.",
      ),
      array(
        'name'   => "obj",
        'type'   => Variant,
        'desc'   => "Which class object this method was calling on.",
      ),
      array(
        'name'   => "params",
        'type'   => VariantVec,
        'desc'   => "The parameters original function took.",
      ),
      array(
        'name'   => "data",
        'type'   => Variant,
        'desc'   => "Information for RPC request in a format of array('host' => address, 'port' => port, 'auth' => password, 'timeout' => seconds).",
      ),
      array(
        'name'   => "done",
        'type'   => Variant | Reference,
        'desc'   => "Will always set to TRUE.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_renamed_functions",
    'desc'   => "Specifies a list of functions that are allowed to rename by calling fb_rename_function().",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => null,
    ),
    'args'   => array(
      array(
        'name'   => "names",
        'type'   => StringVec,
        'desc'   => "The functions that can be renamed.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_rename_function",
    'desc'   => "Rename a function, so that a function can be called with the new name.\n\nWhen writing unit tests, one may want to stub out a function. To do so, call fb_rename_function('func_to_stub_out', 'somename') then fb_rename_function('new_func_to_replace_with', 'func_to_stub_out'). This way, when calling func_to_stub_out(), it will actually execute new_func_to_replace_with().",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Boolean,
      'desc'   => "TRUE if successful, FALSE otherwise.",
    ),
    'args'   => array(
      array(
        'name'   => "orig_func_name",
        'type'   => String,
        'desc'   => "Which function to rename.",
      ),
      array(
        'name'   => "new_func_name",
        'type'   => String,
        'desc'   => "What is the new name.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_utf8ize",
    'desc'   => "Sanitize a string to make sure it's legal UTF-8 by stripping off any characters that are not properly encoded.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Boolean,
      'desc'   => "Sanitized string.",
    ),
    'args'   => array(
      array(
        'name'   => "input",
        'type'   => Variant | Reference,
        'desc'   => "What string to sanitize.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_call_user_func_safe",
    'desc'   => "This function invokes \$function with the arguments specified in its parameter list. It returns an array of two elements, the first being a boolean specifying whether or not the function was invoked, the latter being the return value, or null if it was not invoked. The function may be any PHP callable, either a string function name, an array of object instance and method, or array of classname and static class method.",
    'flags'  =>  HasDocComment | HipHopSpecific | VariableArguments | HasOptFunction | NoProfile,
    'opt'    => "hphp_opt_fb_call_user_func",
    'return' => array(
      'type'   => VariantVec,
      'desc'   => "Two elements, 0 is a bool whether function was invoked, 1 is the return value if invoked.",
    ),
    'args'   => array(
      array(
        'name'   => "function",
        'type'   => Variant,
        'desc'   => "The callback to invoke.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_call_user_func_safe_return",
    'desc'   => "This function invokes \$function with the arguments specified in its parameter\nlist. If the function is not defined, \$default_rval is returned. Note that\nthe default return value comes BEFORE the arguments to the function.",
    'flags'  =>  HasDocComment | HipHopSpecific | VariableArguments | HasOptFunction | NoProfile,
    'opt'    => "hphp_opt_fb_call_user_func",
    'return' => array(
      'type'   => Variant,
      'desc'   => "The result of the function call if defined, otherwise default.",
    ),
    'args'   => array(
      array(
        'name'   => "function",
        'type'   => Variant,
        'desc'   => "The callback to invoke.",
      ),
      array(
        'name'   => "def",
        'type'   => Variant,
        'desc'   => "Value returned when function does not exist.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_call_user_func_array_safe",
    'desc'   => "This function invokes \$function with the arguments specified in its parameter list. It returns an array of two elements, the first being a boolean specifying whether or not the function was invoked, the latter being the return value, or null if it was not invoked. The function may be any PHP callable, either a string function name, an array of object instance and method, or array of classname and static class method.",
    'flags'  =>  HasDocComment | HipHopSpecific | HasOptFunction | NoProfile,
    'opt'    => "hphp_opt_fb_call_user_func",
    'return' => array(
      'type'   => VariantVec,
      'desc'   => "Two elements, 0 is a bool whether function was invoked, 1 is the return value if invoked.",
    ),
    'args'   => array(
      array(
        'name'   => "function",
        'type'   => Variant,
        'desc'   => "The callback to invoke.",
      ),
      array(
        'name'   => "params",
        'type'   => VariantVec,
        'desc'   => "The function parameters to invoke with.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_get_code_coverage",
    'desc'   => "Returns code coverage data collected so far. Turn on code coverage by Eval.RecordCodeCoverage and call this function periodically to get results. Eval.CodeCoverageOutputFile allows you to specify an output file to store results at end of a script run from command line. Use this function in server mode to collect results instead.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Variant,
    ),
    'args'   => array(
      array(
        'name'   => "flush",
        'type'   => Boolean,
        'desc'   => "Whether to clear data after this function call.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "xhprof_enable",
    'desc'   => "Starts xhprof profiling. For details on how to use this function, please refer to http://mirror.facebook.net/facebook/xhprof/doc.html.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => null,
    ),
    'args'   => array(
      array(
        'name'   => "flags",
        'type'   => Int32,
        'desc'   => "One of those XHPROF_FLAGS_ constant to specify what to profile.",
      ),
      array(
        'name'   => "args",
        'type'   => StringVec,
        'value'  => "null_array",
        'desc'   => "Extra argument to reserve for future use.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "xhprof_disable",
    'desc'   => "Ends and reports xhprof profiling result. For details on how to use this function, please refer to http://mirror.facebook.net/facebook/xhprof/doc.html.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Variant,
      'desc'   => "Profile result.",
    ),
  ));

DefineFunction(
  array(
    'name'   => "xhprof_network_enable",
    'desc'   => "Starts xhprof network I/O profiling.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => null,
    ),
  ));

DefineFunction(
  array(
    'name'   => "xhprof_network_disable",
    'desc'   => "Ends and reports xhprof network I/O profiling result.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Variant,
      'desc'   => "Profile result.",
    ),
  ));

DefineFunction(
  array(
    'name'   => "xhprof_frame_begin",
    'desc'   => "Starts an artificial frame. Together with xhprof_frame_end(), this times one block of code execution as if it were a function call, allowing people to define arbitrary function boundaries. Prefer to use XhprofFrame classobjects instead of calling this function directly.",
    'flags'  =>  HasDocComment | HipHopSpecific | NoInjection,
    'return' => array(
      'type'   => null,
    ),
    'args'   => array(
      array(
        'name'   => "name",
        'type'   => String,
        'desc'   => "The \"virtual\" function's name.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "xhprof_frame_end",
    'desc'   => "Ends an artificial frame that xhprof_frame_begin() started. One has to make sure there are no exceptions in between these two calls, as otherwise, it may report incorrect timings. Also, xhprof_frame_begin() and xhprof_frame_end() have to be paired up really well, so not to interfere with regular function's profiling, unless that's the intention. Prefer to use XhprofFrame classobjects instead of calling this function directly.",
    'flags'  =>  HasDocComment | HipHopSpecific | NoInjection,
    'return' => array(
      'type'   => null,
    ),
  ));

DefineFunction(
  array(
    'name'   => "xhprof_run_trace",
    'desc'   => "Re-runs a prior trace and generates xhprof output.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Variant,
      'desc'   => "Profile result.",
    ),
    'args'   => array(
      array(
        'name'   => "packedTrace",
        'type'   => String,
        'desc'   => "A packed trace returned from xhprof_disable.",
      ),
      array(
        'name'   => "flags",
        'type'   => Int32,
        'desc'   => "One of those XHPROF_FLAGS_ constant to specify what to profile.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "xhprof_sample_enable",
    'desc'   => "Starts sampling based xhprof profiling. For details on how to use this function, please refer to http://mirror.facebook.net/facebook/xhprof/doc.html.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => null,
    ),
  ));

DefineFunction(
  array(
    'name'   => "xhprof_sample_disable",
    'desc'   => "Ends sampling based xhprof profiling. For details on how to use this function, please refer to http://mirror.facebook.net/facebook/xhprof/doc.html.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Variant,
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_load_local_databases",
    'desc'   => "Load a map from database IDs (32-bit integers) to database connection information for fb_crossall_query(), which only takes database IDs.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => null,
    ),
    'args'   => array(
      array(
        'name'   => "servers",
        'type'   => VariantMap,
        'desc'   => "The map to load. The format is array(\$id1 => array('ip' => {ip address}, 'db' => {database name}, 'port' => {port number}, 'username' => {user name for login}, 'password' => {password for login}), \$id2 => ...), where \$id1 and \$ids2 are arbitrary 32-bit integers that fb_crossall_query() can use to refer to the physical databases.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_parallel_query",
    'desc'   => "Runs MySQL queries in parallel. Internally it will spawn threads, each of which executes one or more queries sequentially.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => VariantMap,
      'desc'   => "Query result in a format of array('affected' => {number of affected rows}, 'result' => array({dbId1} => {dataset}, {dbId2} => ...), 'error' => array({dbId1} => {error message}, {dbId2} => ....), 'errno' => array({dbId1} => {error code}, {dbId2} => ....)).",
    ),
    'args'   => array(
      array(
        'name'   => "sql_map",
        'type'   => VariantMap,
        'desc'   => "The map to locate databases to query. The format is array(\$id1 => array('sql' => { SQL query to execute}, 'ip' => {ip address}, 'db' => {database name}, 'port' => {port number}, 'username' => {user name for login}, 'password' => {password for login}), \$id2 => ...), where \$id1 and \$ids2 are arbitrary 32-bit integers that fb_crossall_query() can use to refer to the physical databases.",
      ),
      array(
        'name'   => "max_thread",
        'type'   => Int32,
        'value'  => "50",
        'desc'   => "Maximum number of threads to run.",
      ),
      array(
        'name'   => "combine_result",
        'type'   => Boolean,
        'value'  => "true",
        'desc'   => "Whether to merge dataset into just one, without maps of database IDs. If TRUE, result set would be array(..., 'result' => {dataset}, ...) directly.",
      ),
      array(
        'name'   => "retry_query_on_fail",
        'type'   => Boolean,
        'value'  => "true",
        'desc'   => "Whether or not retry query once when it fails. This may be useful if database connection is temporarily lost during queries and re-executing a SELECT or other idempotent queries are acceptable.",
      ),
      array(
        'name'   => "connect_timeout",
        'type'   => Int32,
        'value'  => "-1",
        'desc'   => "Connection timeout. If timeout_in_ms is TRUE, it is in seconds. Otherwise, it is in milli-seconds.",
      ),
      array(
        'name'   => "read_timeout",
        'type'   => Int32,
        'value'  => "-1",
        'desc'   => "Query timeout. If timeout_in_ms is TRUE, it is in seconds. Otherwise, it is in milli-seconds.",
      ),
      array(
        'name'   => "timeout_in_ms",
        'type'   => Boolean,
        'value'  => "false",
        'desc'   => "Whether connect_timeout or read_timeout are in seconds or milli-seconds.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_crossall_query",
    'desc'   => "Runs a MySQL query against all databases in the map loaded by fb_load_local_databases(). Internally it will spawn threads, each of which executes the same query against one or more databases sequentially.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => VariantMap,
      'desc'   => "Query result in a format of array('affected' => {number of affected rows}, 'result' => array({dbId1} => {dataset}, {dbId2} => ...), 'error' => array({dbId1} => {error message}, {dbId2} => ....), 'errno' => array({dbId1} => {error code}, {dbId2} => ....)).",
    ),
    'args'   => array(
      array(
        'name'   => "sql",
        'type'   => String,
        'desc'   => "The SQL query to execute. For safety reasons, this has to be a SELECT statement with WHERE clause.",
      ),
      array(
        'name'   => "max_thread",
        'type'   => Int32,
        'value'  => "50",
        'desc'   => "Maximum number of threads to run.",
      ),
      array(
        'name'   => "retry_query_on_fail",
        'type'   => Boolean,
        'value'  => "true",
        'desc'   => "Whether or not retry query once when it fails. This may be useful if database connection is temporarily lost during queries and re-executing a SELECT or other idempotent queries are acceptable.",
      ),
      array(
        'name'   => "connect_timeout",
        'type'   => Int32,
        'value'  => "-1",
        'desc'   => "Connection timeout. If timeout_in_ms is TRUE, it is in seconds. Otherwise, it is in milli-seconds.",
      ),
      array(
        'name'   => "read_timeout",
        'type'   => Int32,
        'value'  => "-1",
        'desc'   => "Query timeout. If timeout_in_ms is TRUE, it is in seconds. Otherwise, it is in milli-seconds.",
      ),
      array(
        'name'   => "timeout_in_ms",
        'type'   => Boolean,
        'value'  => "false",
        'desc'   => "Whether connect_timeout or read_timeout are in seconds or milli-seconds.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_set_taint",
    'desc'   => "Taints a string with a flag or a bit. This bit is contagious in string operations, being carried over to new strings that are copied or composed from this string. This may be used for checking dirty or clean status of end user's input for different purposes.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => null,
    ),
    'args'   => array(
      array(
        'name'   => "str",
        'type'   => Variant | Reference,
        'desc'   => "The string to taint.",
      ),
      array(
        'name'   => "taint",
        'type'   => Int32,
        'desc'   => "The bit to flag.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_unset_taint",
    'desc'   => "Untaints a string by clearing off the bit that was set or carried over.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => null,
    ),
    'args'   => array(
      array(
        'name'   => "str",
        'type'   => Variant | Reference,
        'desc'   => "The string to untaint.",
      ),
      array(
        'name'   => "taint",
        'type'   => Int32,
        'desc'   => "The bit to clear.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_get_taint",
    'desc'   => "Checks to see if a bit is set.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Int32,
      'desc'   => "All bits that were tainted.",
    ),
    'args'   => array(
      array(
        'name'   => "str",
        'type'   => String,
        'desc'   => "The string to check.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_const_fetch",
    'desc'   => "Fetches a constant string from a special store that's compiled into the executable. This is faster than apc_fetch(), which needs locking between different threads. This store is immutable and loaded once with strings that are never changed. Therefore, no locking is needed when accessed by different threads.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Variant,
      'desc'   => "The value that was stored.",
    ),
    'args'   => array(
      array(
        'name'   => "key",
        'type'   => Variant,
        'desc'   => "The key for locating the value.",
      ),
    ),
  ));


DefineFunction(
  array(
    'name'   => "fb_output_compression",
    'desc'   => "Toggles the compression status of HipHop output, if headers have already been sent this may be ignored.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => Boolean,
      'desc'   => "The old value.",
    ),
    'args'   => array(
      array(
        'name'   => "new_value",
        'type'   => Boolean,
        'desc'   => "The new value for the compression state.",
      ),
    ),
  ));

DefineFunction(
  array(
    'name'   => "fb_set_exit_callback",
    'desc'   => "Set a callback function that is called when php tries to exit.",
    'flags'  =>  HasDocComment | HipHopSpecific,
    'return' => array(
      'type'   => null,
    ),
    'args'   => array(
      array(
        'name'   => "function",
        'type'   => Variant,
        'desc'   => "The callback to invoke. An exception object will be passed to the function",
      ),
    ),
  ));


///////////////////////////////////////////////////////////////////////////////
// Classes
//
// BeginClass
// array (
//   'name'   => name of the class
//   'desc'   => description of the class's purpose
//   'flags'  => attributes of the class, see base.php for possible values
//   'note'   => additional note about this class's schema
//   'parent' => parent class name, if any
//   'ifaces' => array of interfaces tihs class implements
//   'bases'  => extra internal and special base classes this class requires
//   'footer' => extra C++ inserted at end of class declaration
// )
//
// DefineConstant(..)
// DefineConstant(..)
// ...
// DefineFunction(..)
// DefineFunction(..)
// ...
// DefineProperty
// DefineProperty
//
// array (
//   'name'  => name of the property
//   'type'  => type of the property
//   'flags' => attributes of the property
//   'desc'  => description of the property
//   'note'  => additional note about this property's schema
// )
//
// EndClass()

