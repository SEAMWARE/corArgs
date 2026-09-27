#ifndef CORARGS_CORARGSSTATUS_H_
#define CORARGS_CORARGSSTATUS_H_

// -----------------------------------------------------------------------------
//
// FILE                  CorArgsStatus.h - status code for corArgs functions
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//



// -----------------------------------------------------------------------------
//
// CorArgsStatus -
//
typedef enum CorArgsStatus
{
  CorArgsOk,
  CorArgsBadParam,
  CorArgsValueMissing,
  CorArgsInvalidValue,
  CorArgsInvalidConfigItem,
  CorArgsOutOfMemory,
  CorArgsOutOfBounds,
  CorArgsCharOutOfMinLimit,
  CorArgsShortOutOfMinLimit,
  CorArgsIntOutOfMinLimit,
  CorArgsLongOutOfMinLimit,
  CorArgsUCharOutOfMinLimit,
  CorArgsUShortOutOfMinLimit,
  CorArgsUIntOutOfMinLimit,
  CorArgsULongOutOfMinLimit,  // Cannot happen
  CorArgsFloatOutOfMinLimit,
  CorArgsStringOutOfMinLimit,
  CorArgsCharOutOfMaxLimit,
  CorArgsShortOutOfMaxLimit,
  CorArgsIntOutOfMaxLimit,
  CorArgsLongOutOfMaxLimit,
  CorArgsUCharOutOfMaxLimit,
  CorArgsUShortOutOfMaxLimit,
  CorArgsUIntOutOfMaxLimit,
  CorArgsULongOutOfMaxLimit,  // Cannot happen
  CorArgsFloatOutOfMaxLimit,
  CorArgsStringOutOfMaxLimit,
  CorArgsNameTaken
} CorArgsStatus;



// -----------------------------------------------------------------------------
//
// corArgsStatus - convert CorArgsStatus to English text
//
extern char* corArgsStatus(CorArgsStatus status);

#endif  // CORARGS_CORARGSSTATUS_H_
