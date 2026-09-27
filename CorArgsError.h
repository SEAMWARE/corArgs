#ifndef CORARGS_CORARGSERROR_H_
#define CORARGS_CORARGSERROR_H_

// -----------------------------------------------------------------------------
//
// FILE                  CorArgsError.h - CorArgsError struct for error handling
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <errno.h>                      // errno

#include "corArgs/CorArgsStatus.h"      // CorArgsStatus



// -----------------------------------------------------------------------------
//
// CORARGS_ERROR_PUSH -
//
#define CORARGS_ERROR_PUSH(optionName, status, description)                                       \
do                                                                                              \
{                                                                                               \
  KBL_E(("CorArgs Error %s for option '%s': %s", corArgsStatus(status), optionName, description));  \
  corArgsErrorPush(__FILE__, __LINE__, __FUNCTION__, optionName, errno, status, description);     \
} while (0)



// -----------------------------------------------------------------------------
//
// CorArgsError -
//
typedef struct CorArgsError
{
  char*              fileName;
  int                lineNo;
  char*              funcName;
  char*              optionName;
  int                errNo;
  CorArgsStatus      status;
  char*              description;
  struct CorArgsError* next;
} CorArgsError;



// -----------------------------------------------------------------------------
//
// corArgsErrorPush - push an error to the error stack
//
// This is an internal function. It should be used *only* by the internal macro 'CORARGS_ERROR_PUSH'.
//
extern void corArgsErrorPush
(
  const char*  fileName,
  int          lineNo,
  const char*  funcName,
  const char*  optionName,
  int          errNo,
  CorArgsStatus  status,
  const char*  description
);



// -----------------------------------------------------------------------------
//
// corArgsErrorGet -
//
extern CorArgsError* corArgsErrorGet(void);

#endif  // CORARGS_CORARGSERROR_H_
