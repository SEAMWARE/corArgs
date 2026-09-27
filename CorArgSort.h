#ifndef CORARGS_CORARGSORT_H_
#define CORARGS_CORARGSORT_H_

// -----------------------------------------------------------------------------
//
// FILE                  CorArgSort.h - CorArgSort enum for CorArg and CorArgInfo structs
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//



// -----------------------------------------------------------------------------
//
// CorArgSort -
//
typedef enum CorArgSort
{
  CorArgOpt, // Optional
  CorArgReq, // Required
  CorArgHid  // Hidden args are always optional
} CorArgSort;

#endif  // CORARGS_CORARGSORT_H_
