#ifndef CORARGS_CORARGSSECRET_H_
#define CORARGS_CORARGSSECRET_H_

//
// FILE            corArgsSecret.h
//
// Copyright 2026 Seamware
// SPDX-License-Identifier: Apache-2.0
//
#include <stdbool.h>                   // bool



// -----------------------------------------------------------------------------
//
// corArgsSecret - mark an option, by its long name, as one whose value is never shown: a password, a URI that
// may hold credentials. A tool listing the options (coraine's GET /admin/config) masks it.
//
// Called after the option is declared (corArgsInit, or corArgsAdd for a plugin's).
//
// @return false: no such option
//
extern bool corArgsSecret(const char* longName);

#endif  // CORARGS_CORARGSSECRET_H_
