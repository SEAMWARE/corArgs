// -----------------------------------------------------------------------------
//
// FILE                  corArgsPeek.c - peek inside command line arguments
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//

#include <string.h>                    // strcmp
#include "corArgs/CorArg.h"            // CorArg
#include "corArgs/CorArgsError.h"      // CORARGS_ERROR_PUSH
#include "corArgs/corArgsBuiltins.h"   // corArgsBuiltins
#include "corArgs/corArgsPeek.h"       // Own interface



// -----------------------------------------------------------------------------
//
// corArgsPeek - returns the string of the value of the option 'optLongName', if set
//
char* corArgsPeek(int argC, char* argV[], CorArg* kargV, char* optLongName)
{
  if ((argV == NULL) || (kargV == NULL))
  {
    CORARGS_ERROR_PUSH(optLongName, CorArgsBadParam, "NULL parameter");
    return NULL;
  }
  
  // 1. Lookup option in builtins
  CorArg* optP = NULL;
  int   kargIx = 0;

  while (kargV[kargIx].type != CorArgEnd)
  {
    if (strcmp(optLongName, kargV[kargIx].longName) == 0)
    {
      optP = &kargV[kargIx];
      break;
    }

    if (kargV[kargIx].shortName != NULL)
    {
      if (strcmp(optLongName, kargV[kargIx].shortName) == 0)
      {
        optP = &kargV[kargIx];
        break;
      }
    }

    ++kargIx;
  }

  // If not found, Lookup option in builtins
  if (optP == NULL)
  {
    kargIx = 0;
    while (corArgsBuiltins[kargIx].type != CorArgEnd)
    {
      if (strcmp(optLongName, corArgsBuiltins[kargIx].longName) == 0)
      {
        optP = &corArgsBuiltins[kargIx];
        break;
      }
      
      if (corArgsBuiltins[kargIx].shortName != NULL)
      {
        if (strcmp(optLongName, corArgsBuiltins[kargIx].shortName) == 0)
        {
          optP = &corArgsBuiltins[kargIx];
          break;
        }
      }

      ++kargIx;
    }
  }

  if (optP == NULL)
  {
    // Not really an error, just not found ...
    CORARGS_ERROR_PUSH(optLongName, CorArgsBadParam, "no such option");
    return NULL;
  }
  
  // 3. Find option in given arguments (match both long and short names)
  for (int ix = 0; ix < argC; ix++)
  {
    if ((strcmp(optP->longName, argV[ix]) == 0) ||
        (optP->shortName != NULL && strcmp(optP->shortName, argV[ix]) == 0))
    {
      if (optP->type == CorArgBool)
        return "SET";

      if (ix == argC - 1)  // Error: no value for non-bool option
      {
        CORARGS_ERROR_PUSH(optP->longName, CorArgsBadParam, "no value for non-bool option");
        return NULL;
      }

      return argV[ix + 1];
    }
  }

  return NULL;  // option not found in argV
}
