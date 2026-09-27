#ifndef CORARGS_CORARGINFO_H_
#define CORARGS_CORARGINFO_H_

// -----------------------------------------------------------------------------
//
// FILE                  CorArgInfo.h - CorArgInfo struct to hold info on karg options
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//

#include <stdbool.h>                   // bool

#include "corArgs/CorArgType.h"        // CorArgType, CorArgValueFrom
#include "corArgs/CorArgSort.h"        // CorArgSort



// -----------------------------------------------------------------------------
//
// CorArgInfo -
//
typedef struct CorArgInfo
{
  char*          longName;       // Long name of the option, e.g. --port
  char*          shortName;      // MUST start with '-' or '+'
  CorArgType     type;           // type: bool, int, string, etc
  void*          valueP;         // points to where to store the value
  CorArgSort     sort;           // sort: optional, required, hidden
  void*          def;            // default value
  void*          min;            // lower limit
  void*          max;            // upper limit
  char*          description;    // textual description for usage

  bool           builtin;
  bool           disabled;
  CorArgValueFrom  from;
  char           envVar[60];
} CorArgInfo;



// -----------------------------------------------------------------------------
//
// CORARGS_INFO_END - last item in CorArgInfo vector
//
#define CORARGS_INFO_END { "", "", CorArgEnd, NULL, CorArgReq, 0, 0, 0, NULL, false, false, CorArgFromDefaultValue, { 0 } }

#endif  // CORARGS_CORARGINFO_H_
