#ifndef CORARGS_CORARGSADD_H_
#define CORARGS_CORARGSADD_H_

//
// FILE                  corArgsAdd.h - add arguments after corArgsInit
//
// AUTHOR                Ken Zangelin
//
// Copyright 2026 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include "corArgs/CorArgsStatus.h"     // CorArgsStatus
#include "corArgs/CorArg.h"            // CorArg



// -----------------------------------------------------------------------------
//
// corArgsAdd - add more arguments to the existing corArgInfoV
//
// Must be called after corArgsInit and before corArgsParse.
// The kargV array is not copied — it must remain valid.
//
extern CorArgsStatus corArgsAdd(CorArg* kargV);

#endif  // CORARGS_CORARGSADD_H_
