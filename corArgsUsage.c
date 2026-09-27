// -----------------------------------------------------------------------------
//
// FILE                  corArgsUsage.c - handling of the usage CLI
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdio.h>                     // printf, NULL
#include <string.h>                    // strdup
#include <stdbool.h>                   // bool

#include "corBase/corLibLog.h"         // COR_LIB_*

#include "corArgs/CorArgInfo.h"        // CorArgInfo
#include "corArgs/corArgsGlobals.h"    // corArgsProgName, etc
#include "corArgs/corArgsBuiltins.h"   // corArgsBuiltins
#include "corArgs/corArgsInit.h"       // corArgInfoV
#include "corArgs/corArgsUsage.h"      // Own interface



// -----------------------------------------------------------------------------
//
// corArgsUsage -
//
void corArgsUsage(void)
{
  char spaces[128];
  int  len = 7 + strlen(corArgsProgName) + 2;

  COR_LIB_V("In");

  // Protect against ridiculously long program names
  if (len >= (int) sizeof(spaces))
    len = 2;
      
  memset(spaces, ' ', len);
  spaces[len] = 0;

  int    ix                 = 0;
  bool   usageStringPrinted = false;

  while (corArgInfoV[ix].type != CorArgEnd)
  {
    bool   hasLongName  = true;
    bool   hasShortName = true;

    if (corArgInfoV[ix].type == CorArgSeparator)
    {
      printf("\n");
      printf("%s", spaces);
      if (corArgInfoV[ix].description != NULL)
        printf("%s", corArgInfoV[ix].description);
      printf("\n");
      ++ix;
      continue;
    }

    if ((corArgInfoV[ix].sort == CorArgHid) || (corArgInfoV[ix].disabled == true))
    {
      ++ix;
      continue;
    }

    if ((corArgInfoV[ix].longName == NULL) || (corArgInfoV[ix].longName[0] == 0))
      hasLongName = false;
    if ((corArgInfoV[ix].shortName == NULL) || (corArgInfoV[ix].shortName[0] == 0))
      hasShortName = false;

    if (usageStringPrinted == false)
    {
      printf("Usage: %s  ", corArgsProgName);
      usageStringPrinted = true;
    }
    else
      printf("%s", spaces);

    if (corArgInfoV[ix].sort != CorArgReq)
      printf("[");

    if ((hasLongName == true) && (hasShortName == true))
      printf("%s/%s ", corArgInfoV[ix].longName, corArgInfoV[ix].shortName);
    else
      printf("%s ", hasLongName? corArgInfoV[ix].longName : corArgInfoV[ix].shortName);

    if (corArgInfoV[ix].type == CorArgBool)
      printf("(%s)", corArgInfoV[ix].description);
    else
      printf("<%s>", corArgInfoV[ix].description);

    if (corArgInfoV[ix].sort != CorArgReq)
      printf("]");

    printf("\n");

    ++ix;
  }
}



// -----------------------------------------------------------------------------
//
// maxCharsInLongName -
//
static int maxCharsInLongName(void)
{
  int maxLen = 0;
  int ix     = 0;

  while (corArgInfoV[ix].type != CorArgEnd)
  {
    // A separator is not an option: it has no names at all (longName NULL) and
    // is only there to group the listing. Skipping it here also keeps strlen
    // away from that NULL — which is what used to crash --Usage/-U for every
    // program whose parameter table uses CORARGS_SEPARATOR.
    if (corArgInfoV[ix].type == CorArgSeparator)
    {
      ++ix;
      continue;
    }

    const char* longName = corArgInfoV[ix].longName;
    int         len      = (longName != NULL)? strlen(longName) : 0;

    if (len > maxLen)
      maxLen = len;

    ++ix;
  }

  return maxLen;
}



// -----------------------------------------------------------------------------
//
// maxCharsInShortName -
//
static int maxCharsInShortName(void)
{
  int maxLen = 0;
  int ix     = 0;

  while (corArgInfoV[ix].type != CorArgEnd)
  {
    if (corArgInfoV[ix].type == CorArgSeparator)
    {
      ++ix;
      continue;
    }

    const char* shortName = corArgInfoV[ix].shortName;
    int         len       = (shortName != NULL)? strlen(shortName) : 0;

    if (len > maxLen)
      maxLen = len;

    ++ix;
  }

  return maxLen;
}



// -----------------------------------------------------------------------------
//
// maxCharsInEnvVar -
//
static int maxCharsInEnvVar(void)
{
  int maxLen = 0;
  int ix     = 0;
  
  while (corArgInfoV[ix].type != CorArgEnd)
  {
    int len = strlen(corArgInfoV[ix].envVar);

    if (len > maxLen)
      maxLen = len;

    ++ix;
  }

  return maxLen;
}



// -----------------------------------------------------------------------------
//
// typeName - 
//
static char* typeName(CorArgType type)
{
  switch (type)
  {
  case CorArgBool:    return "Bool";
  case CorArgFloat:   return "Float";
  case CorArgString:  return "String";
  case CorArgChar:    return "Char";
  case CorArgUChar:   return "UChar";
  case CorArgShort:   return "Short";
  case CorArgUShort:  return "UShort";
  case CorArgInt:     return "Int";
  case CorArgUInt:    return "UInt";
  case CorArgLong:    return "Long";
  case CorArgULong:   return "ULong";
  case CorArgEnd:     return "No Type";
  case CorArgSeparator:   return "Separator";
  }

  return "No Type";
}



// -----------------------------------------------------------------------------
//
// sortName - 
//
static char* sortName(CorArgSort sort)
{
  switch (sort)
  {
  case CorArgOpt: return "Optional";
  case CorArgReq: return "Required";
  case CorArgHid: return "Hidden";
  }

  return "No Sort";
}



// -----------------------------------------------------------------------------
//
// fromName - 
//
static char* fromName(CorArgValueFrom from)
{
  switch (from)
  {
  case CorArgFromDefaultValue: return "Default Value";
  case CorArgFromConfigFile:   return "Config File";
  case CorArgFromEnvVar: return "Env Var";
  case CorArgFromShortOption:  return "Short Option";
  case CorArgFromLongOption:   return "Long Option";
  }

  return "No From";
}



// -----------------------------------------------------------------------------
//
// corArgsExtUsage -
//
// Usage: corArgsTest  [--usage/-u (usage)]
//
// Long-Name Short-Name EnvVar  Type  Sort  From   Value  Def-Val  Min-Val  Max-Val  Is-Builtin Is-Disabled
//
void corArgsExtUsage(void)
{
  char spaces[128];
  int  len = 7 + strlen(corArgsProgName) + 2;

  COR_LIB_V("In");

  // Protect against ridiculously long program names
  if (len >= (int) sizeof(spaces))
    len = 2;
      
  memset(spaces, ' ', len);
  spaces[len] = 0;

  int    ix            = 0;
  int    longNameChars  = maxCharsInLongName();
  int    shortNameChars = maxCharsInShortName();
  int    envVarChars    = maxCharsInEnvVar();

  while (corArgInfoV[ix].type != CorArgEnd)
  {
    char  format[32];
    char* sort = sortName(corArgInfoV[ix].sort);
    char* type = typeName(corArgInfoV[ix].type);
    char* from = fromName(corArgInfoV[ix].from);

    // A separator carries no option, only a heading for the plain usage
    // listing — there are no columns to fill in here.
    if ((corArgInfoV[ix].sort == CorArgHid) || (corArgInfoV[ix].disabled == true) || (corArgInfoV[ix].type == CorArgSeparator))
    {
      ++ix;
      continue;
    }
    
    //
    // One fixed-width column per field. The short name and the parentheses
    // around it are formatted into a buffer FIRST and then padded as a unit —
    // printing them piecemeal is what used to leave every following column
    // starting at a different offset.
    //
    bool   hasLongName = true;
    if ((corArgInfoV[ix].longName == NULL) || (corArgInfoV[ix].longName[0] == 0))
      hasLongName = false;

    // Long Name
    snprintf(format, sizeof(format), "  %%-%ds", longNameChars);
    printf(format, (hasLongName == true)? corArgInfoV[ix].longName : "-");

    // Short Name, parenthesised, padded as one unit
    bool   hasShortName  = true;
    if ((corArgInfoV[ix].shortName == NULL) || (corArgInfoV[ix].shortName[0] == 0))
      hasShortName = false;

    char shortBuf[64];
    if (hasShortName == true)
      snprintf(shortBuf, sizeof(shortBuf), "(%s)", corArgInfoV[ix].shortName);
    else
      shortBuf[0] = 0;

    snprintf(format, sizeof(format), " %%-%ds", shortNameChars + 2);
    printf(format, shortBuf);

    // Env Var
    snprintf(format, sizeof(format), " %%-%ds", envVarChars);
    printf(format, corArgInfoV[ix].envVar);

    // Sort
    printf(" %-10s", sort);

    // Type
    printf(" %-8s", type);

    // From
    printf(" %-14s ", from);

    printf("\n");

    ++ix;
  }
}
