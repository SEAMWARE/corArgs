#ifndef CORARGS_CORARGSPEEK_H_
#define CORARGS_CORARGSPEEK_H_

// -----------------------------------------------------------------------------
//
// FILE                  corArgsPeek.h - peek inside command line arguments
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include "corArgs/CorArg.h"            // CorArg



// -----------------------------------------------------------------------------
//
// corArgsPeek - returns the string of the value of the option 'optLongName', is set
//
extern char* corArgsPeek(int argC, char* argV[], CorArg* kargV, char* optLongName);

#endif  // CORARGS_CORARGSPEEK_H_
