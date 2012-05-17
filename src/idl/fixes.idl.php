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
    'name'   => "cal_days_in_month",
    'flags'  =>  HasDocComment,
    'return' => array(
      'type'   => Int64,
    ),
    'args'   => array(
      array(
        'name'   => "calendar",
        'type'   => Int64,
      ),
      array(
        'name'   => "month",
        'type'   => Int64,
      ),
      array(
        'name'   => "year",
        'type'   => Int64,
      ),
     )
  ));

DefineConstant(
  array(
    'name'   => "CAL_GREGORIAN",
    'type'   => Int64,
  ));
