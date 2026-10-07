#ifndef CORARGS_CORARGSPARSE_H_
#define CORARGS_CORARGSPARSE_H_

// -----------------------------------------------------------------------------
//
// FILE                  corArgsParse.h - heart of corArgs library
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include "corArgs/CorArgsStatus.h"     // CorArgsStatus
#include "corArgs/CorArgInfo.h"        // CorArgInfo
#include "corArgs/CorArg.h"            // CorArg



// -----------------------------------------------------------------------------
//
// corArgsParse -
//
extern CorArgsStatus corArgsParse(int argC, char* argV[]);



// -----------------------------------------------------------------------------
//
// corArgsValueSet - a non-bool option's value from its text: parsed, checked (its type's range, the
// option's own limits) and set - the command line's and the environment's one way in
//
extern CorArgsStatus corArgsValueSet(CorArgInfo* kargP, const char* valueText);

#endif  // CORARGS_CORARGSPARSE_H_
