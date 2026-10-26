// -----------------------------------------------------------------------------
//
// FILE                  corArgsGlobals.c - global variables
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include "corArgs/corArgsGlobals.h"    // Own interface



// -----------------------------------------------------------------------------
//
// corArgsProgName -
//
char* corArgsProgName = "prog-name";



// -----------------------------------------------------------------------------
//
// corArgsEnvError, corArgsEnvStatus, corArgsEnvReading - see corArgsGlobals.h
//
char          corArgsEnvError[1024];
CorArgsStatus corArgsEnvStatus  = CorArgsOk;
bool          corArgsEnvReading = false;
