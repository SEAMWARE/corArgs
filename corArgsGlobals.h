#ifndef CORARGS_CORARGSGLOBALS_H_
#define CORARGS_CORARGSGLOBALS_H_

// -----------------------------------------------------------------------------
//
// FILE                  corArgsGlobals.h - global variables of the corArgs library
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdbool.h>                    // bool

#include "corArgs/CorArgsStatus.h"     // CorArgsStatus



// -----------------------------------------------------------------------------
//
// corArgsProgName -
//
extern char* corArgsProgName;



// -----------------------------------------------------------------------------
//
// corArgsEnvError - the first value of the environment that was refused, the text that says so; corArgsParse
// says it after its -u/-U check, so a bad CORAINE_PORT does not keep the usage from being printed.
// corArgsEnvStatus: its status, CorArgsOk for none. corArgsEnvReading: the environment is being read -
// corArgsSay writes into corArgsEnvError instead of stdout
//
extern char          corArgsEnvError[1024];
extern CorArgsStatus corArgsEnvStatus;
extern bool          corArgsEnvReading;

#endif  // CORARGS_CORARGSGLOBALS_H_


