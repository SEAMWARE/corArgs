#ifndef CORARGS_CORARGSBUILTINS_H_
#define CORARGS_CORARGSBUILTINS_H_

// -----------------------------------------------------------------------------
//
// FILE                  corArgsBuiltins.h - builtin options of the corArgs library
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//

#include <stdbool.h>                   // bool

#include "corArgs/CorArg.h"            // CorArg



// -----------------------------------------------------------------------------
//
// Variables to hold builtins
//
extern bool   corArgsBuiltinUsage;
extern bool   corArgsBuiltinExtUsage;
extern bool   corArgsBuiltinVerbose;
extern bool   corArgsBuiltinDebug;



// -----------------------------------------------------------------------------
//
// corArgsBuiltins -
//
extern CorArg corArgsBuiltins[];

#endif  // CORARGS_CORARGSBUILTINS_H_
