#ifndef CORARGS_CORARG_H_
#define CORARGS_CORARG_H_

// -----------------------------------------------------------------------------
//
// FILE                  CorArg.h - CorArg struct for the corArgs library
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdio.h>                      // NULL

#include "corArgs/CorArgType.h"         // CorArgType, CorArgValueFrom
#include "corArgs/CorArgSort.h"         // CorArgSort



// -----------------------------------------------------------------------------
//
// CORARGS_END - last item in CorArg vector
//
#define CORARGS_END { "", "", CorArgEnd, NULL, CorArgReq, 0, 0, 0, NULL }
#define CORARGS_SEPARATOR(title) { NULL, NULL, CorArgSeparator, NULL, CorArgHid, NULL, NULL, NULL, title }



// -----------------------------------------------------------------------------
//
// CORARGS_NL - no limit
//
#define CORARGS_NL_NUMBER  0x7FFFFFFFFFFFFFFF
#define CORARGS_NL    (void*) CORARGS_NL_NUMBER



// -----------------------------------------------------------------------------
//
// CorArg -
//
typedef struct CorArg
{
  const char*  longName;       // Long name of the option, e.g. --port
  const char*  shortName;      // MUST start with '-' or '+'
  CorArgType   type;           // type: bool, int, string, etc
  void*        valueP;         // points to where to store the value
  CorArgSort   sort;           // sort: optional, required, hidden
  void*        def;            // default value
  void*        min;            // lower limit
  void*        max;            // upper limit
  const char*  description;    // textual description for usage
} CorArg;

#define _vp (void*)

#endif  // CORARGS_CORARG_H_
