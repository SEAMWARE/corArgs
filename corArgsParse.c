// -----------------------------------------------------------------------------
//
// FILE                  corArgsParse.c - parse command line arguments
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//

//
//
// ToDo
// - fix mess CorArg/corArg/corArgs
// - Mac & Windows are case insensitive for files and functions etc ...
// - corArgsParse - make sure no option is there twice
// - char* corArgsVersion(void)
// - Parameters (not options)
//     * Int or String list with min-max no of items
// - Implement corArgsConfig(CorArgsDisable "-v") and add a test case for it
// - Implement corArgsConfig(CorArgsDisableAllBuiltins, NULL) and add a test case for it
// - corArgsConfig(CorArgsEnvVarName, "-v", "VERBOSE")
//     Check both long and short name
// - Two more types for CorArgsType:
//     * CorArgIList
//     * CorArgSList
//     - Their min and max value are integers that describe min and max items in vector
// - Config File
// - JSON Config File?
// - For all external functions:  if corArgsInit() hasn't been called: ERROR
// - NEW rules for option dependencies
//   - if 'option X' is set 'option Y' CANNOT be set:    corArgsConfig(CorArgsMutuallyExclusive, "-f", "-y")
//   - if 'option X' is set 'option Y' MUST ALSO be set: corArgsConfig((CorArgsPair, "-f", "-y")
// - logrotate, different ways
//
#include <stdio.h>                     // printf
#include <stdlib.h>                    // exit, strtoll
#include <errno.h>                     // errno
#include <limits.h>                    // min/max values for integer types
#include <stdbool.h>                   // bool

#include "kbase/kBasicLog.h"           // KBL_*
#include "kbase/kStringSort.h"         // kStringSort

#include "corArgs/corArgsGlobals.h"    // corArgsProgName, etc
#include "corArgs/CorArgsStatus.h"     // CorArgsStatus
#include "corArgs/CorArgInfo.h"        // CorArgInfo
#include "corArgs/CorArgType.h"        // CorArgType, CorArgValueFrom
#include "corArgs/CorArgSort.h"        // CorArgSort
#include "corArgs/CorArg.h"            // CorArg
#include "corArgs/corArgsInit.h"       // corArgInfoV
#include "corArgs/corArgsConfig.h"     // Config vars
#include "corArgs/corArgsUsage.h"      // corArgsUsage
#include "corArgs/corArgsBuiltins.h"   // corArgsBuiltins
#include "corArgs/CorArgsError.h"      // CORARGS_ERROR_PUSH
#include "corArgs/corArgsParse.h"      // Own interface



// -----------------------------------------------------------------------------
//
// max and min values according to types
//
#define MIN_CHAR   SCHAR_MIN
#define	MAX_CHAR   SCHAR_MAX
#define	MAX_UCHAR  UCHAR_MAX

#define MIN_SHORT  SHRT_MIN
#define MAX_SHORT  SHRT_MAX
#define	MAX_USHORT USHRT_MAX

#define MIN_INT    INT_MIN
#define MAX_INT    INT_MAX
#define MAX_UINT   UINT_MAX

#define MIN_LONG   LLONG_MIN
#define MAX_LONG   LLONG_MAX
#define MAX_ULONG  ULLONG_MAX



// -----------------------------------------------------------------------------
//
// isBoolOpt -
//
static bool isBoolOpt(char shortName, CorArgInfo* kargV)
{
  int argIx = 0;

  while (kargV[argIx].type != CorArgEnd)
  {
    if ((kargV[argIx].shortName != NULL) && (kargV[argIx].shortName[0] == '-') && (shortName == kargV[argIx].shortName[1]))
      return true;

    ++argIx;
  }

  return false;
}



// -----------------------------------------------------------------------------
//
// boolOptSet -
//
static void boolOptSet(char shortName, CorArgInfo* kiV)
{
  int argIx = 0;

  while (kiV[argIx].type != CorArgEnd)
  {
    if ((kiV[argIx].shortName != NULL) && (kiV[argIx].shortName[0] == '-') && (shortName == kiV[argIx].shortName[1]))
    {
      *((bool*) kiV[argIx].valueP) = true;
      kiV[argIx].from = CorArgFromShortOption;

      return;
    }

    ++argIx;
  }
}



// -----------------------------------------------------------------------------
//
// minMaxValueCheck -
//
// Checking max/min values depending of type of argument
//
// Imagine you have an option -c that is of type CorArgChar but used like this:
//
// xxx -c 34005
//
// This must give an error, or course
//
static CorArgsStatus minMaxValueCheck(CorArgInfo* kargP, long long intValue, unsigned long long uintValue)
{
  (void) uintValue;  // the unsigned types are range-checked through intValue
  if ((kargP->type == CorArgInt8) && ((intValue < MIN_CHAR) || (intValue > MAX_CHAR)))
  {
    printf("%s: value %lld out of range for an int8 option (%s)\n", corArgsProgName, intValue, kargP->longName);
    return CorArgsOutOfBounds;
  }
  else if ((kargP->type == CorArgInt16) && ((intValue < MIN_SHORT) || (intValue > MAX_SHORT)))
  {
    printf("%s: value %lld out of range for an int16 option (%s)\n", corArgsProgName, intValue, kargP->longName);
    return CorArgsOutOfBounds;
  }
  else if ((kargP->type == CorArgInt32) && ((intValue < MIN_INT) || (intValue > MAX_INT)))
  {
    printf("%s: value %lld out of range for an int32 option (%s)\n", corArgsProgName, intValue, kargP->longName);
    return CorArgsOutOfBounds;
  }
  else if ((kargP->type == CorArgUInt8) && ((intValue < 0) || (intValue > MAX_UCHAR)))
  {
    printf("%s: value %lld out of range for a uint8 option (%s)\n", corArgsProgName, intValue, kargP->longName);
    return CorArgsOutOfBounds;
  }
  else if ((kargP->type == CorArgUInt16) && ((intValue < 0) || (intValue > MAX_USHORT)))
  {
    printf("%s: value %lld out of range for a uint16 option (%s)\n", corArgsProgName, intValue, kargP->longName);
    return CorArgsOutOfBounds;
  }
  else if ((kargP->type == CorArgUInt32) && ((intValue < 0) || (intValue > MAX_UINT)))
  {
    printf("%s: value %lld out of range for a uint32 option (%s)\n", corArgsProgName, intValue, kargP->longName);
    return CorArgsOutOfBounds;
  }

  return CorArgsOk;
}



// -----------------------------------------------------------------------------
//
// limitCheck - check the limits the user has set for the option
//
CorArgsStatus limitCheck(CorArgInfo* kargP, long long intValue, unsigned long long uintValue, float floatValue, char* stringValue)
{
  if (kargP->min != CORARGS_NL)
  {
    if ((kargP->type == CorArgInt8) ||(kargP->type == CorArgInt16) ||(kargP->type == CorArgInt32) ||(kargP->type == CorArgInt64))
    {
      if (intValue < (long long) kargP->min)
      {
        if (kargP->max != CORARGS_NL)
        {
          printf("%s: value %lld for option '%s' is inferior to its lower limit. Allowed range: %lld-%lld\n",
                 corArgsProgName,
                 intValue,
                 kargP->longName,
                 (long long) kargP->min, (long long) kargP->max);
        }
        else
          printf("%s: value %lld for option '%s' is inferior to %lld, its lower limit\n",
                 corArgsProgName,
                 intValue,
                 kargP->longName,
                 (long long) kargP->min);
                 
        if      (kargP->type == CorArgInt8)   return CorArgsCharOutOfMinLimit;
        else if (kargP->type == CorArgInt16)  return CorArgsShortOutOfMinLimit;
        else if (kargP->type == CorArgInt32)  return CorArgsIntOutOfMinLimit;
        else                              return CorArgsLongOutOfMinLimit;
      }
    }
    else if ((kargP->type == CorArgUInt8) ||(kargP->type == CorArgUInt16) ||(kargP->type == CorArgUInt32) ||(kargP->type == CorArgUInt64))
    {
      if (uintValue < (unsigned long long) kargP->min)
      {
        if (kargP->max != CORARGS_NL)
          printf("%s: value %lld for option '%s' is inferior to its lower limit. Allowed range: %lld-%lld\n",
                 corArgsProgName,
                 intValue,
                 kargP->longName,
                 (long long) kargP->min, (long long) kargP->max);
        else
          printf("%s: value %lld for option '%s' is inferior to %lld, its lower limit\n",
                 corArgsProgName,
                 intValue,
                 kargP->longName,
                 (long long) kargP->min);

        if      (kargP->type == CorArgUInt8)   return CorArgsUCharOutOfMinLimit;
        else if (kargP->type == CorArgUInt16)  return CorArgsUShortOutOfMinLimit;
        else if (kargP->type == CorArgUInt32)  return CorArgsUIntOutOfMinLimit;
        else                               return CorArgsULongOutOfMinLimit;  // Not possible
      }
    }
    else if (kargP->type == CorArgFloat)
    {
      if (floatValue < (float) (long long) kargP->min)
      {
        if (kargP->max != CORARGS_NL)
          printf("%s: value %f for option '%s' is inferior to its lower limit. Allowed range: %f-%f\n",
                 corArgsProgName,
                 floatValue,
                 kargP->longName,
                 (float) (long long) kargP->min, (float) (long long) kargP->max);
        else
          printf("%s: value %f for option '%s' is inferior to %f, its lower limit\n",
                 corArgsProgName,
                 floatValue,
                 kargP->longName,
                 (float) (long long) kargP->min);
          
        return CorArgsFloatOutOfMinLimit;
      }
    }
    else if (kargP->type == CorArgString)
    {
      if (strcmp(stringValue, (char*) kargP->min) < 0)
      {
        if (kargP->max != CORARGS_NL)
          printf("%s: value '%s' for option '%s' is inferior to its lower limit. Allowed range: '%s' - '%s'\n",
                 corArgsProgName,
                 stringValue,
                 kargP->longName,
                 (char*) kargP->min, (char*) kargP->max);
        else
          printf("%s: value '%s' for option '%s' is inferior to '%s', its lower limit\n",
                 corArgsProgName,
                 stringValue,
                 kargP->longName,
                 (char*) kargP->min);
          
        return CorArgsStringOutOfMinLimit;
      }
    }
  }

  if (kargP->max != CORARGS_NL)
  {
    if ((kargP->type == CorArgInt8) ||(kargP->type == CorArgInt16) ||(kargP->type == CorArgInt32) ||(kargP->type == CorArgInt64))
    {
      if (intValue > (long long) kargP->max)
      {
        if (kargP->min != CORARGS_NL)
          printf("%s: value %lld for option '%s' exceeds its upper limit. Allowed range: %lld-%lld\n",
                 corArgsProgName,
                 intValue,
                 kargP->longName,
                 (long long) kargP->min, (long long) kargP->max);
        else
          printf("%s: value %lld for option '%s' exceeds %lld, its upper limit\n",
                 corArgsProgName,
                 intValue,
                 kargP->longName,
                 (long long) kargP->max);

        if      (kargP->type == CorArgInt8)   return CorArgsCharOutOfMaxLimit;
        else if (kargP->type == CorArgInt16)  return CorArgsShortOutOfMaxLimit;
        else if (kargP->type == CorArgInt32)  return CorArgsIntOutOfMaxLimit;
        else                              return CorArgsLongOutOfMaxLimit;
      }
    }
    else if ((kargP->type == CorArgUInt8) ||(kargP->type == CorArgUInt16) ||(kargP->type == CorArgUInt32) ||(kargP->type == CorArgUInt64))
    {
      if (uintValue > (unsigned long long) kargP->max)
      {
        if (kargP->min != CORARGS_NL)
          printf("%s: value %lld for option '%s' exceeds its upper limit. Allowed range: %llu-%llu\n",
                 corArgsProgName,
                 intValue,
                 kargP->longName,
                 (unsigned long long) kargP->min, (unsigned long long) kargP->max);
        else
          printf("%s: value %lld for option '%s' exceeds %llu, its upper limit\n",
                 corArgsProgName,
                 intValue,
                 kargP->longName,
                 (unsigned long long) kargP->max);

        if      (kargP->type == CorArgUInt8)   return CorArgsUCharOutOfMaxLimit;
        else if (kargP->type == CorArgUInt16)  return CorArgsUShortOutOfMaxLimit;
        else if (kargP->type == CorArgUInt32)  return CorArgsUIntOutOfMaxLimit;
        else                               return CorArgsULongOutOfMaxLimit;  // Not possible
      }
    }
    else if (kargP->type == CorArgFloat)
    {
      if (floatValue > (float) (long long) kargP->max)
      {
        if (kargP->min != CORARGS_NL)
          printf("%s: value %f for option '%s' exceeds its upper limit. Allowed range: %f-%f\n",
                 corArgsProgName,
                 floatValue,
                 kargP->longName,
                 (float) (long long) kargP->min, (float) (long long) kargP->max);
        else
          printf("%s: value %f for option '%s' exceeds %f, its upper limit\n",
                 corArgsProgName,
                 floatValue,
                 kargP->longName,
                 (float) (long long) kargP->max);
          
        return CorArgsFloatOutOfMaxLimit;
      }
    }
    else if (kargP->type == CorArgString)
    {
      if (strcmp(stringValue, (char*) kargP->max) < 0)
      {
        if (kargP->min != CORARGS_NL)
          printf("%s: value '%s' for option '%s' exceeds its upper limit. Allowed range: '%s' - '%s'\n",
                 corArgsProgName,
                 stringValue,
                 kargP->longName,
                 (char*) kargP->min, (char*) kargP->max);
        else
          printf("%s: value '%s' for option '%s' exceeds '%s', its upper limit\n",
                 corArgsProgName,
                 stringValue,
                 kargP->longName,
                 (char*) kargP->min);
          
        return CorArgsStringOutOfMaxLimit;
      }
    }
  }

  return CorArgsOk;
}



// -----------------------------------------------------------------------------
//
// optionSet -
//
static CorArgsStatus optionSet(CorArgInfo* kargP, int argC, char* argV[], int argIx)
{
  long long           intValue    = 0;
  unsigned long long  uintValue   = 0;
  float               floatValue  = 0;
  char*               stringValue = NULL;
  CorArgsStatus       ks;

  KBL_V(("Setting option '%s', argIx: %d, argC: %d", kargP->longName, argIx, argC));

  if ((kargP->type != CorArgBool) && (argIx + 1 >= argC))
  {
    printf("%s: value missing for option '%s'\n", corArgsProgName, kargP->longName);
    return CorArgsValueMissing;
  }

  if (kargP->type == CorArgBool)
  {
    *((bool*) kargP->valueP) = true;
    return CorArgsOk;
  }    

  if (kargP->type == CorArgFloat)
  {
    char* endP = NULL;
    
    errno      = 0;
    floatValue = strtof(argV[argIx + 1], &endP);

    if (errno == ERANGE)
    {
      printf("%s: invalid value for float option '%s'\n", corArgsProgName, kargP->longName);
      return CorArgsInvalidValue;
    }
  }
  else if (kargP->type == CorArgString)
    stringValue = argV[argIx + 1];
  else
  {
    int base = 10;

    //
    // Starts with "0", "0x", "H'", "O'", "B'" ?
    //
    // 0x123:  Hexadecimal
    // H'123:  Hexadecimal
    // O'123:  Octadecimal
    // B'101:  Binary
    //
    // 0123:   Octadecimal  ?
    //
    char* valueP = argV[argIx + 1];
    char* rest   = NULL;

    if ((valueP[0] == '0') && ((valueP[1] == 'x') || (valueP[1] == 'X')))
    {
      valueP = &valueP[2];
      base   = 16;
    }
    else if (((valueP[0] == 'H') || (valueP[0] == 'h')) && (valueP[1] == '\''))
    {
      valueP = &valueP[2];
      base   = 16;
    }
    else if (((valueP[0] == 'O') || (valueP[0] == 'o')) && (valueP[1] == '\''))
    {
      valueP = &valueP[2];
      base   = 8;
    }
    else if (((valueP[0] == 'B') || (valueP[0] == 'b')) && (valueP[1] == '\''))
    {
      valueP = &valueP[2];
      base   = 2;
    }

    errno     = 0;
    intValue  = strtoll(valueP,  &rest, base);
    uintValue = strtoull(valueP, &rest, base);

    // KBL_M(("valueP: '%s', base: %d, intValue: %d", valueP, base, intValue));

    if (errno == ERANGE)
    {
      printf("%s: invalid value (%s) for integer option '%s'\n", corArgsProgName, valueP, kargP->longName);
      return CorArgsOutOfBounds;
    }

    if ((ks = minMaxValueCheck(kargP, intValue, uintValue)) != CorArgsOk)
      return ks;

    if ((ks = limitCheck(kargP, intValue, uintValue, floatValue, stringValue)) != CorArgsOk)
      return ks;
  }

  //
  // All good, set the value
  //
  if      (kargP->type == CorArgInt8) *((char*)               kargP->valueP) = (char)               intValue;
  else if (kargP->type == CorArgUInt8)    *((unsigned char*)      kargP->valueP) = (unsigned char)      uintValue;
  else if (kargP->type == CorArgInt16)    *((short*)              kargP->valueP) = (short)              intValue;
  else if (kargP->type == CorArgUInt16)   *((unsigned short*)     kargP->valueP) = (unsigned short)     uintValue;
  else if (kargP->type == CorArgInt32)    *((int*)                kargP->valueP) = (int)                intValue;
  else if (kargP->type == CorArgUInt32)   *((unsigned int*)       kargP->valueP) = (unsigned int)       uintValue;
  else if (kargP->type == CorArgInt64)    *((long long*)          kargP->valueP) = (long long)          intValue;
  else if (kargP->type == CorArgUInt64)   *((unsigned long long*) kargP->valueP) = (unsigned long long) uintValue;
  else if (kargP->type == CorArgFloat)    *((float*)              kargP->valueP) = floatValue;
  else if (kargP->type == CorArgString)   *((char**)              kargP->valueP) = stringValue;

  return CorArgsOk;
}



// -----------------------------------------------------------------------------
//
// configFileValues -
//
static CorArgsStatus configFileValues(CorArgInfo* kiV)
{
  (void) kiV;
  return CorArgsOk;
}



// -----------------------------------------------------------------------------
//
// envVarValues -
//
static CorArgsStatus envVarValues(CorArgInfo* kiV)
{
  (void) kiV;
  return CorArgsOk;
}



// -----------------------------------------------------------------------------
//
// corArgsParse -
//
CorArgsStatus corArgsParse(int argC, char* argV[])
{
  int          argIx = 1;
  CorArgsStatus  ks    = CorArgsOk;

  //
  // Set values from config file
  //
  ks = configFileValues(corArgInfoV);
  if (ks != CorArgsOk)
  {
    CORARGS_ERROR_PUSH("global", ks, "configFileValues error");
    return ks;
  }
  
  //
  // Set values from env vars
  //
  ks = envVarValues(corArgInfoV);
  if (ks != CorArgsOk)
  {
    CORARGS_ERROR_PUSH("global", ks, "envVarValues error");
    return ks;
  }


  //
  // Set values from command line
  //
  while (argIx < argC)
  {
    int   kargIx = 0;
    bool found   = false;
    
    while (corArgInfoV[kargIx].type != CorArgEnd)
    {
      char* optName;

      if ((corArgInfoV[kargIx].shortName != NULL) && (strcmp(argV[argIx], corArgInfoV[kargIx].shortName) == 0))
      {
        found = true;
        corArgInfoV[kargIx].from = CorArgFromShortOption;
        optName = (char*) corArgInfoV[kargIx].shortName;
      }
      else if ((corArgInfoV[kargIx].longName != NULL) && (strcmp(argV[argIx], corArgInfoV[kargIx].longName) == 0))
      {
        found = true;
        corArgInfoV[kargIx].from = CorArgFromLongOption;
        optName = (char*) corArgInfoV[kargIx].longName;
      }

      if (found == true)
      {
        KBL_V(("Found option '%s': %s", optName, corArgInfoV[kargIx].description));
        if ((ks = optionSet(&corArgInfoV[kargIx], argC, argV, argIx)) != 0)
        {
          CORARGS_ERROR_PUSH(optName, ks, corArgsStatus(ks));
          return ks;
        }

        //
        // If not bool option, then the next CLI param is consumed as well
        //
        if (corArgInfoV[kargIx].type != CorArgBool)
          ++argIx;

        break;
      }
      ++kargIx;
    }

    //
    // Concatenated bool options? (like 'ltr' in "ls -ltr")
    //
    if ((found == false) && (argV[argIx][0] == '-'))
    {
      char*  charsSorted  = strdup(&argV[argIx][1]); // Remove the initial '-'
      char*  toFree       = charsSorted;
      bool   allOk        = true;

      kStringSort(charsSorted);

      while (*charsSorted != 0)
      {
        // Check for repeats
        if (charsSorted[0] == charsSorted[1])
        {
          printf("%s: repeated boolean option: -%c\n", corArgsProgName, charsSorted[0]);
          exit(1);
        }

        if (isBoolOpt(*charsSorted, corArgInfoV) == false)
        {
          allOk = false;
          break;
        }
        ++charsSorted;
      }

      if (allOk == true)
      {
        found = true;

        charsSorted = &argV[argIx][1];

        while (*charsSorted != 0)
        {
          boolOptSet(*charsSorted, corArgInfoV);
          ++charsSorted;
        }
      }

      free(toFree);
    }


    if (found == false)
    {
      printf("%s: unrecognized option: %s\n", corArgsProgName, argV[argIx]);
      corArgsUsage();
      exit(1);
    }

    ++argIx;
  }


  //
  // Actions for builtins
  //
  if (corArgsBuiltinUsage == true)
  {
    corArgsUsage();

    if (corArgsExitOnUsage == true)
      exit(corArgsExitOnUsageExitCode);
  }

  if (corArgsBuiltinExtUsage == true)
  {
    corArgsExtUsage();

    if (corArgsExitOnUsage == true)
      exit(corArgsExitOnUsageExitCode);
  }

  return CorArgsOk;
}
