/* -*- mode: c; -*- */

enum result_t {
  SUCCESS =  0,
  ERROR   = -1
};

program INDICATOR_PROGRAM
{
  version INDICATOR_VERSION
  {
    result_t set_status (int) = 1;
  } = 0;
} = 25552;

