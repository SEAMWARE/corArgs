// -----------------------------------------------------------------------------
//
// FILE                  CorArgsError.c - CorArgsError struct for error handling
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdio.h>                     // NULL
#include <stdlib.h>                    // calloc

#include "corBase/corLibLog.h"         // COR_LIB_*

#include "corArgs/CorArgsStatus.h"     // CorArgsStatus
#include "corArgs/CorArgsError.h"      // Own interface



// -----------------------------------------------------------------------------
//
// errorListHead - 
//
static CorArgsError* errorListHead = NULL;



// -----------------------------------------------------------------------------
//
// corArgsErrorPush - push an error to the error stack
//
// This is an internal function. It should be used *only* by the internal macro 'CORARGS_ERROR_PUSH'.
//
void corArgsErrorPush
(
  const char*  fileName,
  int          lineNo,
  const char*  funcName,
  const char*  optionName,
  int          errNo,
  CorArgsStatus  status,
  const char*  description
)
{
  CorArgsError* eP = (CorArgsError*) calloc(1, sizeof(CorArgsError));

  if (eP == NULL)  // Out of memory ...
    COR_LIB_RVE("Out Of Memory");

  eP->fileName     = (char*) fileName;
  eP->lineNo       = lineNo;
  eP->funcName     = (char*) funcName;
  eP->optionName   = (char*) optionName;
  eP->errNo        = errNo;
  eP->status       = status;
  eP->description  = (char*) description;

  // First error item?
  if (errorListHead == NULL)
  {
    errorListHead = eP;
    errorListHead->next = NULL;
  }
  else
  {
    //
    // Error item are inserted in the beginning of the list, so that
    // the lower level error items comes after in the list
    //
    eP->next      = errorListHead;
    errorListHead = eP;
  }

  COR_LIB_V("%s[%d]: %s: %s", eP->fileName, eP->lineNo, eP->funcName, eP->description);
}



// -----------------------------------------------------------------------------
//
// corArgsErrorGet -
//
CorArgsError* corArgsErrorGet(void)
{
  return errorListHead;
}
