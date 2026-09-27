// -----------------------------------------------------------------------------
//
// FILE                  corArgsBuiltins.c - builtin options of the corArgs library
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdlib.h>                    // exit
#include <stdbool.h>                   // bool

#include "corArgs/CorArg.h"            // CorArg, _vp, CORARGS_END
#include "corArgs/corArgsBuiltins.h"   // Own interface



// -----------------------------------------------------------------------------
//
// Variables to hold builtins
//
bool corArgsBuiltinUsage;
bool corArgsBuiltinExtUsage;
bool corArgsBuiltinVerbose;
bool corArgsBuiltinDebug;



// -----------------------------------------------------------------------------
//
// corArgsBuiltins -
//
CorArg corArgsBuiltins[] =
{
  { "--usage",   "-u", CorArgBool, &corArgsBuiltinUsage,    CorArgOpt, _vp false, _vp false, _vp true, "usage",        },
  { "--Usage",   "-U", CorArgBool, &corArgsBuiltinExtUsage, CorArgOpt, _vp false, _vp false, _vp true, "extended usage" },
  { "--verbose", "-v", CorArgBool, &corArgsBuiltinVerbose,  CorArgOpt, _vp false, _vp false, _vp true, "verbose mode"      },
  { "--debug",   "-d", CorArgBool, &corArgsBuiltinDebug,    CorArgOpt, _vp false, _vp false, _vp true, "debug mode" },

  CORARGS_END
};
