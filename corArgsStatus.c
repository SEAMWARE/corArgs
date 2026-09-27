// -----------------------------------------------------------------------------
//
// FILE                  corArgsStatus.c - convert CorArgsStatus to English text
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include "corArgs/CorArgsStatus.h"      // Own interface



// -----------------------------------------------------------------------------
//
// corArgsStatus - convert CorArgsStatus to English text
//
char* corArgsStatus(CorArgsStatus status)
{
  switch (status)
  {
  case CorArgsOk:                   return "ok";
  case CorArgsBadParam:             return "bad parameter";
  case CorArgsValueMissing:         return "value missing";
  case CorArgsInvalidValue:         return "invalid value";
  case CorArgsInvalidConfigItem:    return "invalid config Item";
  case CorArgsOutOfMemory:          return "out of memory";
  case CorArgsOutOfBounds:          return "out of bounds";
  case CorArgsCharOutOfMinLimit:    return "char out of min limit";
  case CorArgsShortOutOfMinLimit:   return "short out of min limit";
  case CorArgsIntOutOfMinLimit:     return "int out of min limit";
  case CorArgsLongOutOfMinLimit:    return "long out of min limit";
  case CorArgsUCharOutOfMinLimit:   return "unsigned char out of min limit";
  case CorArgsUShortOutOfMinLimit:  return "unsigned short out of min limit";
  case CorArgsUIntOutOfMinLimit:    return "unsigned int out of min limit";
  case CorArgsULongOutOfMinLimit:   return "unsigned long out of min limit";
  case CorArgsFloatOutOfMinLimit:   return "float out of min limit";
  case CorArgsStringOutOfMinLimit:  return "string out of min limit";
  case CorArgsCharOutOfMaxLimit:    return "char out of max limit";
  case CorArgsShortOutOfMaxLimit:   return "short out of max limit";
  case CorArgsIntOutOfMaxLimit:     return "int out of max limit";
  case CorArgsLongOutOfMaxLimit:    return "long out of max limit";
  case CorArgsUCharOutOfMaxLimit:   return "unsigned char out of max limit";
  case CorArgsUShortOutOfMaxLimit:  return "unsigned short out of max limit";
  case CorArgsUIntOutOfMaxLimit:    return "unsigned int out of max limit";
  case CorArgsULongOutOfMaxLimit:   return "unsigned long out of max limit";
  case CorArgsFloatOutOfMaxLimit:   return "float out of max limit";
  case CorArgsStringOutOfMaxLimit:  return "string out of max limit";    
  case CorArgsNameTaken:            return "name taken";
  }

  return "Unknown Error";
}
