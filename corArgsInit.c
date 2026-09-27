// -----------------------------------------------------------------------------
//
// FILE                  corArgsInit.c - initialize command line argument parsing
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdlib.h>                    // calloc
#include <stdbool.h>                   // bool

#include "kbase/kBasicLog.h"           // KBL_V
#include "kbase/kMacros.h"             // K_VEC_SIZE

#include "corArgs/CorArgsStatus.h"     // CorArgsStatus
#include "corArgs/CorArgsError.h"      // CORARGS_ERROR_PUSH
#include "corArgs/CorArg.h"            // CorArg
#include "corArgs/corArgsGlobals.h"    // corArgsProgName
#include "corArgs/corArgsBuiltins.h"   // corArgsBuiltins
#include "corArgs/corArgsConfig.h"     // Config vars
#include "corArgs/corArgInfoPopulate.h"    // corArgInfoPopulate
#include "corArgs/CorArgsError.h"      // CORARGS_ERROR_PUSH
#include "corArgs/corArgsInit.h"       // Own interface



// -----------------------------------------------------------------------------
//
// Definitions for min/max values of builtin C integer types
//
#define CHAR_MIN        (char) 0x80
#define CHAR_MAX        (char) 0x7F
#define UCHAR_MIN       (unsigned char) 0
#define UCHAR_MAX       (unsigned char) 0xFF
#define SHORT_MIN       (short) 0x8000
#define SHORT_MAX       (short) 0x7FFF
#define USHORT_MIN      (unsigned short) 0
#define USHORT_MAX      (unsigned short) 0xFFFF
#define INT_MIN         (int) 0x80000000
#define INT_MAX         (int) 0x7FFFFFFF
#define UINT_MIN        (unsigned int) 0
#define UINT_MAX        (unsigned int) 0xFFFFFFFF
#define LONG_MIN        (long long) 0x8000000000000000
#define LONG_MAX        (long long) 0x7FFFFFFFFFFFFFFF
#define ULONG_MIN       (unsigned long long) 0x0000000000000000
#define ULONG_MAX       (unsigned long long) 0xFFFFFFFFFFFFFFFF



// -----------------------------------------------------------------------------
//
// corArgInfoV -
//
CorArgInfo* corArgInfoV;



// -----------------------------------------------------------------------------
//
// outOfBound - 
//
static bool outOfBound
(
  CorArgType          type,
  long long           sVal,
  unsigned long long  uVal,
  char**              errorString
)
{
  switch (type)
  {
  case CorArgBool:
  case CorArgFloat:
  case CorArgString:
  case CorArgEnd:
  case CorArgSeparator:  // a section heading, not an option: no value of any kind
    return false;

  case CorArgChar:
    if (sVal < CHAR_MIN)
    {
      *errorString = "lower than minimum value for type";
      return true;
    }

    if (sVal > CHAR_MAX)
    {
      *errorString = "higher than maximum value for type";
      return true;
    }
    break;

  case CorArgShort:
    if (sVal < SHORT_MIN)
    {
      *errorString = "lower than minimum value for type";
      return true;
    }

    if (sVal > SHORT_MAX)
    {
      *errorString = "higher than maximum value for type";
      return true;
    }
    break;

  case CorArgInt:
    KBL_V(("sVal:    %lld", sVal));
    KBL_V(("INT_MIN: %d", INT_MIN));
    KBL_V(("INT_MAX: %d", INT_MAX));

    if (sVal < INT_MIN)
    {
      *errorString = "lower than minimum value for type";
      return true;
    }

    if (sVal > INT_MAX)
    {
      *errorString = "higher than maximum value for type";
      return true;
    }
    break;

  case CorArgUChar:
    if (sVal < 0)
    {
      *errorString = "lower than minimum value for type";
      return true;
    }

    if (uVal > UCHAR_MAX)
    {
      *errorString = "higher than maximum value for type";
      return true;
    }
    break;

  case CorArgUShort:
    if (sVal < 0)
    {
      *errorString = "lower than minimum value for type";
      return true;
    }
    
    if (uVal > USHORT_MAX)
    {
      *errorString = "higher than maximum value for type";
      return true;
    }
    break;

  case CorArgUInt:
    if (sVal < 0)
    {
      *errorString = "lower than minimum value for type";
      return true;
    }
    
    if (uVal > UINT_MAX)
    {
      *errorString = "higher than maximum value for type";
      return true;
    }
    break;

  case CorArgULong:
  case CorArgLong:
    break;
  }

  return false;   // Meaning: NOT out-of-bounds (== OK)
}



// -----------------------------------------------------------------------------
//
// corArgVectorCheck - check that the user input vector is OK
//
// 1.  No longName nor shortName
// 2.  User option with the same longName/shortName as a builtin (4 combinations)?
// 3.  User option with the same longName/shortName as some other user option (4 combinations)?
// 4.  Default value and range limits within min/max for its type?
// 5.  Default value within its max/min range?
// 6.  Range OK? (min <= max)
// 7.  Invalid CorArgType (compiler should warn - typecasts get thru that)
// 8.  NULL valueP
// 9.  Invalid CorArgSort (compiler should warn - typecasts get thru that)
// 10. NULL description
//
static CorArgsStatus corArgVectorCheck(CorArg* kiV)
{
  int kiIx = 0;

  KBL_V(("In corArgVectorCheck"));
  
  //
  // 1. Any user option with neither longName nor shortName?
  //
  KBL_V(("corArgVectorCheck: Any user option with neither longName nor shortName?"));
  while (kiV[kiIx].type != CorArgEnd)
  {
    if ((kiV[kiIx].longName == NULL) || (kiV[kiIx].longName[0] == 0))
    {
      if ((kiV[kiIx].shortName == NULL) || (kiV[kiIx].shortName[0] == 0))
      {
        CORARGS_ERROR_PUSH("No Name", CorArgsBadParam, "Option without name");
        return CorArgsBadParam;
      }
    }

    ++kiIx;
  }


  //
  // 2. Any user option with the same longName/shortName as a builtin?
  //
  KBL_V(("corArgVectorCheck: Any user option with the same longName/shortName as a builtin?"));
  kiIx = 0;
  while (kiV[kiIx].type != CorArgEnd)
  {
    int bIx = 0;

    while (corArgsBuiltins[bIx].type != CorArgEnd)
    {
      if (kiV[kiIx].longName != NULL)
      {
        if (strcmp(kiV[kiIx].longName, corArgsBuiltins[bIx].longName) == 0)
        {
          CORARGS_ERROR_PUSH(kiV[kiIx].longName, CorArgsNameTaken, "Option long-name already in use (by builtin long-name)");
          return CorArgsNameTaken;
        }
        
        if (strcmp(kiV[kiIx].longName, corArgsBuiltins[bIx].shortName) == 0)
        {
          CORARGS_ERROR_PUSH(kiV[kiIx].longName, CorArgsNameTaken, "Option long-name already in use (by builtin short-name)");
          return CorArgsNameTaken;
        }
      }

      if (kiV[kiIx].shortName != NULL)
      {
        if (strcmp(kiV[kiIx].shortName, corArgsBuiltins[bIx].longName) == 0)
        {
          CORARGS_ERROR_PUSH(kiV[kiIx].shortName, CorArgsNameTaken, "Option short-name already in use (by builtin long-name)");
          return CorArgsNameTaken;
        }

        if (strcmp(kiV[kiIx].shortName, corArgsBuiltins[bIx].shortName) == 0)
        {
          CORARGS_ERROR_PUSH(kiV[kiIx].shortName, CorArgsNameTaken, "Option short-name already in use (by builtin short-name)");
          return CorArgsNameTaken;
        }
      }

      ++bIx;
    }

    ++kiIx;
  }


  //
  // 3. Any user option with the same longName/shortName as some other user option?
  //
  KBL_V(("corArgVectorCheck: Any user option with the same longName/shortName some other user option?"));
  int ix1 = 0;
  while (kiV[ix1].type != CorArgEnd)
  {
    int ix2 = 0;

    while (kiV[ix2].type != CorArgEnd)
    {
      if (ix1 == ix2)  // Will not compare an option to itself
      {
        ++ix2;
        continue;
      }
      
      if (kiV[ix1].longName != NULL)
      {
        KBL_V(("Testing longName '%s'", kiV[ix1].longName));
        if (kiV[ix2].longName != NULL)
        {
          KBL_V(("Checking against longName '%s'", kiV[ix2].longName));
          if (strcmp(kiV[ix1].longName, kiV[ix2].longName) == 0)
          {
            CORARGS_ERROR_PUSH(kiV[kiIx].longName, CorArgsNameTaken, "Option long-name already in use (by other options long-name)");
            return CorArgsNameTaken;
          }
        }

        if (kiV[ix2].shortName != NULL)
        {
          KBL_V(("Checking against shortName '%s'", kiV[ix2].shortName));
          if (strcmp(kiV[ix1].longName, kiV[ix2].shortName) == 0)
          {
            CORARGS_ERROR_PUSH(kiV[kiIx].longName, CorArgsNameTaken, "Option long-name already in use (by other options short-name)");
            return CorArgsNameTaken;
          }
        }
      }

      if (kiV[ix1].shortName != NULL)
      {
        KBL_V(("Testing shortName '%s'", kiV[ix1].shortName));
        if (kiV[ix2].longName != NULL)
        {
          KBL_V(("Checking against longName '%s'", kiV[ix2].longName));
          if (strcmp(kiV[ix1].shortName, kiV[ix2].longName) == 0)
          {
            CORARGS_ERROR_PUSH(kiV[kiIx].shortName, CorArgsNameTaken, "Option short-name already in use (by other options short-name)");
            return CorArgsNameTaken;
          }
        }

        if (kiV[ix2].shortName != NULL)
        {
          KBL_V(("Checking against shortName '%s'", kiV[ix2].shortName));
          if (strcmp(kiV[ix1].shortName, kiV[ix2].shortName) == 0)
          {
            CORARGS_ERROR_PUSH(kiV[kiIx].shortName, CorArgsNameTaken, "Option short-name already in use (by other options short-name)");
            return CorArgsNameTaken;
          }
        }
      }

      ++ix2;
    }

    ++ix1;
  }


  //
  // 4. Default value and range limits within min/max for its type?
  //    E.g. a max-value of 256 for a CorArgUChar is an error as an 8-bit value is 0-255
  //
  KBL_V(("corArgVectorCheck: Default+Range-Limits within min/max for type?"));
  int ix = 0;
  while (kiV[ix].type != CorArgEnd)
  {
    char*               name   = (kiV[ix].longName == NULL)? (char*) kiV[ix].shortName : (char*) kiV[ix].longName;
    long long           sMin   = (long long) kiV[ix].min;
    unsigned long long  uMin   = (unsigned long long) kiV[ix].min;
    long long           sMax   = (long long) kiV[ix].max;
    unsigned long long  uMax   = (unsigned long long) kiV[ix].max;
    long long           sDef   = (long long) kiV[ix].def;
    unsigned long long  uDef   = (unsigned long long) kiV[ix].def;
    char*               errorString;

    // Min limit out of bounds?
    if (sMin != CORARGS_NL_NUMBER)
    {
      KBL_V(("sMin:      %lld (0x%llx)", sMin, sMin));
      KBL_V(("LONG_MIN:  %lld (0x%llx)", LONG_MIN, LONG_MIN));
      KBL_V(("uMin:      %llu (0x%llx)", uMin, uMin));
      KBL_V(("ULONG_MIN: %llu (0x%llx)", ULONG_MIN, ULONG_MIN));
      if (outOfBound(kiV[ix].type, sMin, uMin, &errorString) == true)
      {
        CORARGS_ERROR_PUSH(name, CorArgsBadParam, errorString);
        return CorArgsBadParam;
      }
    }

    // Max limit out of bounds?
    if (sMax != CORARGS_NL_NUMBER)
    {
      if (outOfBound(kiV[ix].type, sMax, uMax, &errorString) == true)
      {
        CORARGS_ERROR_PUSH(name, CorArgsBadParam, errorString);
        return CorArgsBadParam;
      }
    }

    // Default value out of bounds?
    if (sDef != CORARGS_NL_NUMBER)
    {
      if (outOfBound(kiV[ix].type, sDef, uDef, &errorString) == true)
      {
        CORARGS_ERROR_PUSH(name, CorArgsBadParam, errorString);
        return CorArgsBadParam;
      }
    }

    ++ix;
  }        


  //
  // 5. Default value within its range?
  //
  KBL_V(("corArgVectorCheck: Default value within its range?"));
  while (kiV[ix].type != CorArgEnd)
  {
    char*               name   = (kiV[ix].longName == NULL)? (char*) kiV[ix].shortName : (char*) kiV[ix].longName;
    long long           sMin   = (long long) kiV[ix].min;
    unsigned long long  uMin   = (unsigned long long) kiV[ix].min;
    long long           sMax   = (long long) kiV[ix].max;
    unsigned long long  uMax   = (unsigned long long) kiV[ix].max;
    long long           sDef   = (long long) kiV[ix].def;
    unsigned long long  uDef   = (unsigned long long) kiV[ix].def;
    
    switch (kiV[ix].type)
    {
    case CorArgBool:
    case CorArgFloat:
    case CorArgString:
    case CorArgEnd:
    case CorArgSeparator:    // a section heading carries no value to convert
      break;

    case CorArgChar:
    case CorArgShort:
    case CorArgInt:
    case CorArgLong:
      if ((sMin != CORARGS_NL_NUMBER) && (sDef < sMin))
      {
        CORARGS_ERROR_PUSH(name, CorArgsBadParam, "Default value lower than the minimum limit for option");
        return CorArgsBadParam;
      }

      if ((sMax != CORARGS_NL_NUMBER) && (sDef > sMax))
      {
        CORARGS_ERROR_PUSH(name, CorArgsBadParam, "Defaultvalue exceeds the maximum limit for option");
        return CorArgsBadParam;
      }
      break;

    case CorArgUChar:
    case CorArgUShort:
    case CorArgUInt:
    case CorArgULong:
      if ((uMin != CORARGS_NL_NUMBER) && (uDef < uMin))
      {
        CORARGS_ERROR_PUSH(name, CorArgsBadParam, "Default value lower than the minimum limit for option");
        return CorArgsBadParam;
      }
      if ((uMax != CORARGS_NL_NUMBER) && (uDef > uMax))
      {
        CORARGS_ERROR_PUSH(name, CorArgsBadParam, "Default value exceeds the maximum limit for option");
        return CorArgsBadParam;
      }
      break;
    }

    ++ix;
  }


  //
  // 6. Min Limit > Max Limit?
  //    Def <= Max Limit?
  //    Def >= Min Limit?
  //
  KBL_V(("corArgVectorCheck: Min > Max?"));
  ix = 0;
  while (kiV[ix].type != CorArgEnd)
  {
    char*               name   = (kiV[ix].longName == NULL)? (char*) kiV[ix].shortName : (char*) kiV[ix].longName;
    long long           sMin   = (long long) kiV[ix].min;
    unsigned long long  uMin   = (unsigned long long) kiV[ix].min;
    long long           sMax   = (long long) kiV[ix].max;
    unsigned long long  uMax   = (unsigned long long) kiV[ix].max;
    long long           sDef   = (long long) kiV[ix].def;
    unsigned long long  uDef   = (unsigned long long) kiV[ix].def;

    // If any part is unlimited, skip the test
    if ((sMin == CORARGS_NL_NUMBER) || (sMax == CORARGS_NL_NUMBER))
    {
      ++ix;
      continue;
    }

    switch (kiV[ix].type)
    {
    case CorArgBool:
    case CorArgFloat:
    case CorArgString:
    case CorArgEnd:
    case CorArgSeparator:    // a section heading carries no value to convert
      break;

    case CorArgChar:
    case CorArgShort:
    case CorArgInt:
    case CorArgLong:
      if (sMin > sMax)
      {
        CORARGS_ERROR_PUSH(name, CorArgsBadParam, "Minimum limit is higher than the maximum limit for option");
        return CorArgsBadParam;
      }

      if (sDef > sMax)
      {
        CORARGS_ERROR_PUSH(name, CorArgsBadParam, "Default value is higher than the maximum limit for option");
        return CorArgsBadParam;
      }

      if (sDef < sMin)
      {
        CORARGS_ERROR_PUSH(name, CorArgsBadParam, "Default value is lower than the minimum limit for option");
        return CorArgsBadParam;
      }
      break;

    case CorArgUChar:
    case CorArgUShort:
    case CorArgUInt:
    case CorArgULong:
      if (uMin > uMax)
      {
        CORARGS_ERROR_PUSH(name, CorArgsBadParam, "Minimum limit is higher than the maximum limit for option");
        return CorArgsBadParam;
      }

      if (uDef > uMax)
      {
        CORARGS_ERROR_PUSH(name, CorArgsBadParam, "Default value is higher than the maximum limit for option");
        return CorArgsBadParam;
      }

      if (uDef < uMin)
      {
        CORARGS_ERROR_PUSH(name, CorArgsBadParam, "Default value is lower than the minimum limit for option");
        return CorArgsBadParam;
      }
    }
    ++ix;
  }


  //
  // 7. Invalid CorArgType (compiler should warn - typecasts get thru that)
  //
  KBL_V(("corArgVectorCheck: Invalid CorArgType?"));
  ix = 0;
  while (kiV[ix].type != CorArgEnd)
  {
    char* name   = (kiV[ix].longName == NULL)? (char*) kiV[ix].shortName : (char*) kiV[ix].longName;

    if ((kiV[ix].type < CorArgBool) || (kiV[ix].type > CorArgEnd))
    {
      CORARGS_ERROR_PUSH(name, CorArgsBadParam, "Invalid 'type' for option");
      return CorArgsBadParam;
    }
    
    ++ix;
  }


  //
  // 8. NULL valueP
  //
  KBL_V(("corArgVectorCheck: NULL valueP?"));
  ix = 0;
  while (kiV[ix].type != CorArgEnd)
  {
    char* name   = (kiV[ix].longName == NULL)? (char*) kiV[ix].shortName : (char*) kiV[ix].longName;

    if (kiV[ix].valueP == NULL)
    {
      CORARGS_ERROR_PUSH(name, CorArgsBadParam, "NULL value pointer for option");
      return CorArgsBadParam;
    }
    
    ++ix;
  }


  //
  // 9. Invalid CorArgSort (compiler should warn - typecasts get thru that)
  //
  KBL_V(("corArgVectorCheck: Invalid CorArgSort?"));
  ix = 0;
  while (kiV[ix].type != CorArgEnd)
  {
    char* name   = (kiV[ix].longName == NULL)? (char*) kiV[ix].shortName : (char*) kiV[ix].longName;

    if ((kiV[ix].sort < CorArgOpt) || (kiV[ix].sort > CorArgHid))
    {
      CORARGS_ERROR_PUSH(name, CorArgsBadParam, "Invalid 'sort' for option");
      return CorArgsBadParam;
    }

    ++ix;
  }


  //
  // 10. NULL description
  //
  KBL_V(("corArgVectorCheck: NULL description?"));
  ix = 0;
  while (kiV[ix].type != CorArgEnd)
  {
    char* name   = (kiV[ix].longName == NULL)? (char*) kiV[ix].shortName : (char*) kiV[ix].longName;

    if (kiV[ix].description == NULL)
    {
      CORARGS_ERROR_PUSH(name, CorArgsBadParam, "NULL description pointer for option");
      return CorArgsBadParam;
    }

    ++ix;
  }

  return CorArgsOk;
}



// -----------------------------------------------------------------------------
//
// defaultValues -
//
static CorArgsStatus defaultValues(CorArgInfo* kiV)
{
  int kargIx = 0;

  while (kiV[kargIx].type != CorArgEnd)
  {
    CorArgInfo* kargP = &kiV[kargIx];

    switch (kargP->type)
    {
    case CorArgSeparator: break;   // a section heading has no valueP

    case CorArgBool:
      *((bool*) kargP->valueP) = (bool) (long long) kargP->def;
      break;

    case CorArgString:
      *((char**) kargP->valueP) = (char*) kargP->def;
      break;

    case CorArgFloat:
      *((float*) kargP->valueP) = (float) (long long) kargP->def;
      break;

    case CorArgInt8:
      *((char*) kargP->valueP) = (char) (long long) kargP->def;
      break;

    case CorArgUInt8:
      *((unsigned char*) kargP->valueP) = (unsigned char) (long long) kargP->def;
      break;

    case CorArgInt16:
      *((short*) kargP->valueP) = (short) (long long) kargP->def;
      break;

    case CorArgUInt16:
      *((unsigned short*) kargP->valueP) = (unsigned short) (long long) kargP->def;
      break;

    case CorArgInt32:
      *((int*) kargP->valueP) = (int) (long long) kargP->def;
      break;

    case CorArgUInt32:
      *((unsigned int*) kargP->valueP) = (unsigned int) (long long) kargP->def;
      break;

    case CorArgInt64:
      *((long long*) kargP->valueP) = (long long) kargP->def;
      break;

    case CorArgUInt64:
      *((unsigned long long*) kargP->valueP) = (unsigned long long) kargP->def;
      break;

    case CorArgEnd:
      break;
    }

    ++kargIx;
  }

  return CorArgsOk;
}



// -----------------------------------------------------------------------------
//
// envVarValues -
//
static CorArgsStatus envVarValues(CorArgInfo* kiV)
{
  for (int kargIx = 0; kiV[kargIx].type != CorArgEnd; kargIx++)
  {
    CorArgInfo* kargP     = &kiV[kargIx];
    char*     envVarValue = getenv(kargP->envVar);
    bool      bValue      = false;

    if (envVarValue == NULL)
      continue;

    unsigned long long iValue = atoi(envVarValue);

    if (kargP->type == CorArgBool)
    {
      if (strcasecmp(envVarValue, "TRUE") == 0)
        bValue = true;
    }

    switch (kargP->type)
    {
    case CorArgSeparator: break;   // a section heading has no valueP

    case CorArgBool:  *((bool*)               kargP->valueP) = bValue;                              break;
    case CorArgString:    *((char**)              kargP->valueP) = (char*) envVarValue;                 break;
    case CorArgFloat: *((float*)              kargP->valueP) = (float) strtod(envVarValue, NULL);   break;
    case CorArgInt8:  *((char*)               kargP->valueP) = (char)  iValue;                      break;
    case CorArgUInt8: *((unsigned char*)      kargP->valueP) = (unsigned char)  iValue;             break;
    case CorArgInt16: *((short*)              kargP->valueP) = (short) iValue;                      break;
    case CorArgUInt16:    *((unsigned short*)     kargP->valueP) = (unsigned short) iValue;             break;
    case CorArgInt32: *((int*)                kargP->valueP) = (int)   iValue;                      break;
    case CorArgUInt32:    *((unsigned int*)       kargP->valueP) = (unsigned int)   iValue;             break;
    case CorArgInt64: *((long long*)          kargP->valueP) = (long long)      iValue;             break;
    case CorArgUInt64:    *((unsigned long long*) kargP->valueP) = (unsigned long long) iValue;         break;
    case CorArgEnd:
      break;
    }

    kargP->from = CorArgFromEnvVar;
  }

  return CorArgsOk;
}



// -----------------------------------------------------------------------------
//
// corArgsInit -
//
CorArgsStatus corArgsInit(const char* progName, CorArg* kargV, const char* prefix)
{
  CorArgsStatus ks;

  if (kargV == NULL)
  {
    CORARGS_ERROR_PUSH("global", CorArgsBadParam, "NULL argument vector");
    return CorArgsBadParam;
  }
  
  corArgsProgName = (char*) progName;
  corArgsPrefix = (char*) prefix;

  //
  // Check integrity of the CorArg vector.
  // Make sure:
  //   o all min values are lower than the max values,
  //   o the default values are in between limits,
  //   o what more?
  //
  KBL_V(("Calling corArgVectorCheck"));
  ks = corArgVectorCheck(kargV);
  if (ks != CorArgsOk)
  {
    CORARGS_ERROR_PUSH("global", ks, "corArgVectorCheck error");
    return ks;
  }
  

  //
  // Create the CorArgInfo vector from the user defined CorArg vector and the builtins
  //
  int userOptions = 0;
  int builtins    = 0;
  int options     = 0;

  while (corArgsBuiltins[builtins].type != CorArgEnd)
    ++builtins;
  KBL_V(("%d builtins", builtins));

  while (kargV[userOptions].type != CorArgEnd)
    ++userOptions;
  KBL_V(("%d user defined options", userOptions));

  options = builtins + userOptions + 1;

  KBL_V(("Allocating room for %d options", options));
  corArgInfoV = (CorArgInfo*) calloc(options, sizeof(CorArgInfo));

  if (corArgInfoV == NULL)
  {
    CORARGS_ERROR_PUSH("global", CorArgsOutOfMemory, "allocating CorArgInfo array");
    return CorArgsOutOfMemory;
  }
  
  int ix = 0;

  KBL_V(("Calling corArgInfoPopulate for builtins"));
  KBL_V(("Calling corArgInfoPopulate for user defined options"));
  corArgInfoPopulate(corArgInfoV, corArgsBuiltins, true,   &ix);
  corArgInfoPopulate(corArgInfoV, kargV,     false, &ix);

  // Mark the last slot as END
  corArgInfoV[ix].type = CorArgEnd;


  //
  // Set default values for all option variables
  //
  ks = defaultValues(corArgInfoV);
  if (ks != CorArgsOk)
  {
    CORARGS_ERROR_PUSH("global", ks, "error in default values");
    return ks;
  }

  //
  // Set values for env vars
  //
  ks = envVarValues(corArgInfoV);
  if (ks != CorArgsOk)
  {
    CORARGS_ERROR_PUSH("global", ks, "error in env values");
    return ks;
  }
  
  return CorArgsOk;
}
