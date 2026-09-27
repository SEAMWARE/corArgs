// -----------------------------------------------------------------------------
//
// FILE                  corArgInfoPopulate.c - populate the CorArgInfo vector
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <string.h>                    // memset, strcmp, strlen, sprintf
#include <stdbool.h>                   // bool

#include "kbase/kBasicLog.h"           // KBL_*

#include "corArgs/CorArgInfo.h"        // CorArgInfo
#include "corArgs/corArgsConfig.h"     // corArgsPrefix
#include "corArgs/corArgInfoPopulate.h"    // Own interface



// -----------------------------------------------------------------------------
//
// corArgInfoPopulate - populate the CorArgInfo vector
//
void corArgInfoPopulate(CorArgInfo* corArgInfoV, CorArg* kargV, bool builtin, int* ixP)
{
  int ix   = 0;    // Index in kargV (input)
  int kiIx = *ixP; // Index in corArgInfoV (output)

  while (kargV[ix].type != CorArgEnd)
  {
    // Separator: just copy type + description, skip everything else
    if (kargV[ix].type == CorArgSeparator)
    {
      memset(&corArgInfoV[kiIx], 0, sizeof(CorArgInfo));
      corArgInfoV[kiIx].type      = CorArgSeparator;
      corArgInfoV[kiIx].description = (char*) kargV[ix].description;
      ++ix;
      ++kiIx;
      continue;
    }

    corArgInfoV[kiIx].builtin    = builtin;
    corArgInfoV[kiIx].disabled   = false;
    corArgInfoV[kiIx].from       = CorArgFromDefaultValue;

    corArgInfoV[kiIx].longName   = (char*) kargV[ix].longName;
    corArgInfoV[kiIx].shortName  = (char*) kargV[ix].shortName;
    corArgInfoV[kiIx].type       = kargV[ix].type;
    corArgInfoV[kiIx].valueP     = kargV[ix].valueP;
    corArgInfoV[kiIx].sort       = kargV[ix].sort;
    corArgInfoV[kiIx].def        = kargV[ix].def;
    corArgInfoV[kiIx].min        = kargV[ix].min;
    corArgInfoV[kiIx].max        = kargV[ix].max;
    corArgInfoV[kiIx].description  = (char*) kargV[ix].description;


    //
    // Build env var from longName and corArgsPrefix
    //
    // Builtin --usage and --Usage have no env var
    //
    if ((corArgInfoV[kiIx].longName != NULL) && (strcmp(corArgInfoV[kiIx].longName, "--usage") == 0))
    {
      corArgInfoV[kiIx].envVar[0] = 0;

      ++ix;
      ++kiIx;

      continue;
    }
    if ((corArgInfoV[kiIx].longName != NULL) && (strcmp(corArgInfoV[kiIx].longName, "--Usage") == 0))
    {
      corArgInfoV[kiIx].envVar[0] = 0;

      ++ix;
      ++kiIx;

      continue;
    }
    
    int   prefixLen   = (corArgsPrefix == NULL)? 0 : strlen(corArgsPrefix);
    int   onIx        = 0;  // index of optName
    bool lname        = (corArgInfoV[kiIx].longName == NULL)? false : true;
    char* optName     = (lname == true)? (char*) corArgInfoV[kiIx].longName   : (char*) corArgInfoV[kiIx].shortName;

    if (corArgsPrefix != NULL)
      sprintf(corArgInfoV[kiIx].envVar, "%s", corArgsPrefix);

    // If double '-' initially, skip one of the hyphens
    if ((optName[0] == '-') && (optName[1] == '-'))
      ++optName;

    while (prefixLen + onIx < (int) sizeof(corArgInfoV[kiIx].envVar))
    {
      char c = optName[onIx];

      if (c == 0)
        break;

      //
      // toupper + avoid forbidden chars.
      // Also, first char cannot be a digit if prefix is empty
      // All non [a-zA-Z0-0_] are transformed to underscores
      //
      if ((c >= 'A') && (c <= 'Z'))
        ;  // Keep as is
      else if ((c >= 'a') && (c <= 'z'))
        c = c - ('a' - 'A');
      else if ((onIx == 0) &&  (corArgsPrefix[0] == 0) && ((c >= '0') && (c <= '9')))
        c = '_';
      else if (c == '_')
        ;  // OK
      else if (c == '-')
        c = '_';  // OK
      else if ((c >= '0') && (c <= '9'))
        ;  // OK
      else
        c = '_';

      corArgInfoV[kiIx].envVar[prefixLen + onIx] = c;

      ++onIx;
    }

    corArgInfoV[kiIx].envVar[prefixLen + onIx] = 0;
    KBL_V(("Env var for '%s': %s", corArgInfoV[kiIx].longName, corArgInfoV[kiIx].envVar));

    ++ix;
    ++kiIx;
  }

  *ixP = kiIx;
}
