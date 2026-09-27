#ifndef CORARGS_CORARGSCONFIG_H_
#define CORARGS_CORARGSCONFIG_H_

// -----------------------------------------------------------------------------
//
// FILE                  corArgsConfig.h - configure corArgs library
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdbool.h>                   // bool

#include "corArgs/CorArgsStatus.h"     // CorArgsStatus



// -----------------------------------------------------------------------------
//
// corArgsConfig -
//
typedef enum CorArgsConfigItem
{
  CorArgsPrefix,
  CorArgsExitOnUsage,
  CorArgsExitOnUsageExitCode
} CorArgsConfigItem;




// -----------------------------------------------------------------------------
//
// Config vars
//
extern char* corArgsPrefix;
extern bool corArgsExitOnUsage;
extern int   corArgsExitOnUsageExitCode;



// -----------------------------------------------------------------------------
//
// corArgsConfig -
//
extern CorArgsStatus corArgsConfig(CorArgsConfigItem cItem, char* value);

#endif  // CORARGS_CORARGSCONFIG_H_
