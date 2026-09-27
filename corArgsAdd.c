// -----------------------------------------------------------------------------
//
// FILE                  corArgsAdd.c - add arguments after corArgsInit
//
// AUTHOR                Ken Zangelin
//
// Copyright 2026 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdlib.h>                    // realloc
#include <stdbool.h>                   // bool


#include "corArgs/CorArgsStatus.h"     // CorArgsStatus
#include "corArgs/CorArgsError.h"      // CORARGS_ERROR_PUSH
#include "corArgs/CorArg.h"            // CorArg
#include "corArgs/CorArgInfo.h"        // CorArgInfo
#include "corArgs/corArgInfoPopulate.h"    // corArgInfoPopulate
#include "corArgs/corArgsInit.h"       // corArgInfoV
#include "corArgs/corArgsAdd.h"        // Own interface



// -----------------------------------------------------------------------------
//
// corArgsAdd -
//
CorArgsStatus corArgsAdd(CorArg* kargV)
{
  if (kargV == NULL)
    return CorArgsOk;

  if (corArgInfoV == NULL)
  {
    CORARGS_ERROR_PUSH("global", CorArgsBadParam, "corArgsAdd called before corArgsInit");
    return CorArgsBadParam;
  }

  // Count existing entries
  int existing = 0;
  while (corArgInfoV[existing].type != CorArgEnd)
    existing++;

  // Count new entries
  int newCount = 0;
  while (kargV[newCount].type != CorArgEnd)
    newCount++;

  if (newCount == 0)
    return CorArgsOk;

  // Realloc to fit existing + new + 1 (for CorArgEnd sentinel)
  int total = existing + newCount + 1;
  CorArgInfo* newV = (CorArgInfo*) realloc(corArgInfoV, total * sizeof(CorArgInfo));

  if (newV == NULL)
  {
    CORARGS_ERROR_PUSH("global", CorArgsOutOfMemory, "realloc in corArgsAdd");
    return CorArgsOutOfMemory;
  }

  corArgInfoV = newV;

  // Populate the new entries starting at 'existing'
  int ix = existing;
  corArgInfoPopulate(corArgInfoV, kargV, false, &ix);

  // Mark end
  corArgInfoV[ix].type = CorArgEnd;

  // Set default values for the new entries (skip separators)
  for (int i = existing; i < ix; i++)
  {
    CorArgInfo* kargP = &corArgInfoV[i];

    if (kargP->type == CorArgSeparator)
      continue;

    switch (kargP->type)
    {
    case CorArgSeparator: break;   // skipped above; named so the switch is exhaustive

    case CorArgBool:  *((bool*)               kargP->valueP) = (bool) (long long) kargP->def;    break;
    case CorArgString:    *((char**)              kargP->valueP) = (char*) kargP->def;                break;
    case CorArgFloat: *((float*)              kargP->valueP) = (float) (long long) kargP->def;    break;
    case CorArgChar:  *((char*)               kargP->valueP) = (char)  (long long) kargP->def;    break;
    case CorArgUChar: *((unsigned char*)      kargP->valueP) = (unsigned char)  (long long) kargP->def;  break;
    case CorArgShort: *((short*)              kargP->valueP) = (short) (long long) kargP->def;    break;
    case CorArgUShort:    *((unsigned short*)     kargP->valueP) = (unsigned short) (long long) kargP->def;  break;
    case CorArgInt:   *((int*)                kargP->valueP) = (int)   (long long) kargP->def;    break;
    case CorArgUInt:  *((unsigned int*)       kargP->valueP) = (unsigned int)   (long long) kargP->def;  break;
    case CorArgLong:  *((long long*)          kargP->valueP) = (long long) kargP->def;            break;
    case CorArgULong: *((unsigned long long*) kargP->valueP) = (unsigned long long) kargP->def;   break;
    case CorArgEnd:   break;
    }
  }

  return CorArgsOk;
}
