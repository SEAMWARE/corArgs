#ifndef CORARGS_CORARGINFOPOPULATE_H_
#define CORARGS_CORARGINFOPOPULATE_H_

// -----------------------------------------------------------------------------
//
// FILE                  corArgInfoPopulate.h - populate the CorArgInfo vector
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include "corArgs/CorArgInfo.h"        // CorArgInfo
#include "corArgs/CorArg.h"            // CorArg



// -----------------------------------------------------------------------------
//
// corArgInfoPopulate - populate the CorArgInfo vector
//
extern void corArgInfoPopulate(CorArgInfo* corArgInfoV, CorArg* kargV, bool builtin, int* ixP);

#endif  // CORARGS_CORARGINFOPOPULATE_H_
