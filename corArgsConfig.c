// -----------------------------------------------------------------------------
//
// FILE                  corArgsConfig.c - configure corArgs library
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <string.h>                    // strdup
#include <stdlib.h>                    // atoi
#include <stdbool.h>                   // bool

#include "kbase/kBasicLog.h"           // KBL_*

#include "corArgs/CorArgsStatus.h"     // CorArgsStatus
#include "corArgs/CorArgsError.h"      // CORARGS_ERROR_PUSH
#include "corArgs/corArgsConfig.h"     // Own interface



// -----------------------------------------------------------------------------
//
// Config vars
//
char* corArgsPrefix            = "";
bool corArgsExitOnUsage        = true;
int   corArgsExitOnUsageExitCode = 0;



// -----------------------------------------------------------------------------
//
// corArgsConfig -
//
CorArgsStatus corArgsConfig(CorArgsConfigItem cItem, char* value)
{
  switch (cItem)
  {
  case CorArgsPrefix:
    corArgsPrefix = strdup(value);
    break;
    
  case CorArgsExitOnUsageExitCode:
    corArgsExitOnUsageExitCode = atoi(value);
    break;

  case CorArgsExitOnUsage:
    if ((strcasecmp(value, "yes") == 0) || (strcasecmp(value, "on") == 0))
      corArgsExitOnUsage = true;
    else if ((strcasecmp(value, "no") == 0) || (strcasecmp(value, "off") == 0))
      corArgsExitOnUsage = false;
    else
    {
      CORARGS_ERROR_PUSH("ExitOnUsage", CorArgsInvalidConfigItem, value);
      return CorArgsInvalidConfigItem;
    }
    break;
  }

  return CorArgsOk;
}
