//
// FILE            corArgsSecret.c
//
// Copyright 2026 Seamware
// SPDX-License-Identifier: Apache-2.0
//
#include <stdbool.h>                   // bool
#include <string.h>                    // strcmp

#include "corArgs/CorArgInfo.h"        // CorArgInfo
#include "corArgs/corArgsInit.h"       // corArgInfoV

#include "corArgs/corArgsSecret.h"     // Own interface



// -----------------------------------------------------------------------------
//
// corArgsSecret -
//
bool corArgsSecret(const char* longName)
{
  if ((corArgInfoV == NULL) || (longName == NULL))
    return false;

  for (int ix = 0; corArgInfoV[ix].type != CorArgEnd; ix++)
  {
    if ((corArgInfoV[ix].longName != NULL) && (strcmp(corArgInfoV[ix].longName, longName) == 0))
    {
      corArgInfoV[ix].secret = true;
      return true;
    }
  }

  return false;
}
