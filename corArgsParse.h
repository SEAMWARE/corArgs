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
#include "corArgs/CorArg.h"            // CorArg



// -----------------------------------------------------------------------------
//
// corArgsParse -
//
extern CorArgsStatus corArgsParse(int argC, char* argV[]);

#endif  // CORARGS_CORARGSPARSE_H_
