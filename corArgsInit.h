#ifndef CORARGS_CORARGSINIT_H_
#define CORARGS_CORARGSINIT_H_

// -----------------------------------------------------------------------------
//
// FILE                  corArgsInit.h - initialization of the corArgs library
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include "corArgs/CorArgInfo.h"        // CorArgInfo
#include "corArgs/CorArgsStatus.h"     // CorArgsStatus
#include "corArgs/CorArg.h"            // CorArg



// -----------------------------------------------------------------------------
//
// corArgInfoV - move to some other module
//
extern CorArgInfo* corArgInfoV;



// -----------------------------------------------------------------------------
//
// corArgsInit -
//
extern CorArgsStatus corArgsInit(const char* progName, CorArg* kargV, const char* prefix);

#endif  // CORARGS_CORARGSINIT_H_
