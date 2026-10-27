//
// FILE            limitsTest.c
//
// AUTHOR          Ken Zangelin
//
// Copyright 2026 Seamware
// SPDX-License-Identifier: Apache-2.0
//
// The program test/limitsTest.sh runs: an option table chosen by the first argument, given to corArgsInit,
// then the rest of the command line to corArgsParse.
//
//   limits      - a string with limits '050'-'100' (--str), a float with limits 1-10 (--rate),
//                 a section heading (CORARGS_SEPARATOR)
//   defOverMax  - an int whose default (20) is above its only limit, a max of 10
//   defNoLimit  - an int with a max of 10 and no default (CORARGS_NL): fine
//   dupLong     - two options called --dup
//
//   limitsTest <table> [options]
//
// Once parsed: "OK str <str> rate <rate>", exit 0. corArgsInit refusing the table: "corArgsInit: <status>
// option '<name>'", exit 2. corArgsParse refusing a value: corArgs's own output, exit 3.
//
#include <stdio.h>                               // printf
#include <string.h>                              // strcmp
#include <stdbool.h>                             // bool

#include "corArgs/corArgs.h"                     // corArgsInit, corArgsParse
#include "corArgs/CorArg.h"                      // CorArg
#include "corArgs/CorArgsError.h"                // corArgsErrorGet



static char*  str;
static float  rate;
static int    count;

static CorArg limits[] =
{
  CORARGS_SEPARATOR("Limited"),
  { "--str",   NULL, CorArgString, &str,   CorArgOpt, "060",   (void*) "050", (void*) "100", "a string, '050'-'100'" },
  { "--rate",  NULL, CorArgFloat,  &rate,  CorArgOpt, (void*) 2, (void*) 1,   (void*) 10,    "a float, 1-10" },
  CORARGS_END
};

static CorArg defOverMax[] =
{
  { "--count", NULL, CorArgInt,    &count, CorArgOpt, (void*) 20, CORARGS_NL, (void*) 10,    "an int, at most 10" },
  CORARGS_END
};

static CorArg defNoLimit[] =
{
  { "--count", NULL, CorArgInt,    &count, CorArgOpt, CORARGS_NL, CORARGS_NL, (void*) 10,    "an int, at most 10" },
  CORARGS_END
};

static CorArg dupLong[] =
{
  { "--one",   NULL, CorArgInt,    &count, CorArgOpt, (void*) 1, CORARGS_NL, CORARGS_NL,     "one" },
  { "--dup",   NULL, CorArgInt,    &count, CorArgOpt, (void*) 1, CORARGS_NL, CORARGS_NL,     "dup" },
  { "--dup",   NULL, CorArgInt,    &count, CorArgOpt, (void*) 1, CORARGS_NL, CORARGS_NL,     "dup again" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// main -
//
int main(int argC, char* argV[])
{
  CorArg* tableP = NULL;

  if (argC < 2)
    return 1;

  if      (strcmp(argV[1], "limits")     == 0)  tableP = limits;
  else if (strcmp(argV[1], "defOverMax") == 0)  tableP = defOverMax;
  else if (strcmp(argV[1], "defNoLimit") == 0)  tableP = defNoLimit;
  else if (strcmp(argV[1], "dupLong")    == 0)  tableP = dupLong;
  else
    return 1;

  CorArgsStatus ks = corArgsInit("limitsTest", tableP, "LIMITSTEST");
  if (ks != CorArgsOk)
  {
    CorArgsError* eP = corArgsErrorGet();  // the latest first: the check's own error is the last one

    while ((eP != NULL) && (eP->next != NULL))
      eP = eP->next;

    printf("corArgsInit: %s option '%s'\n", corArgsStatus(ks), ((eP != NULL) && (eP->optionName != NULL))? eP->optionName : "(null)");
    return 2;
  }

  argV[1] = argV[0];
  if (corArgsParse(argC - 1, &argV[1]) != CorArgsOk)
    return 3;

  printf("OK str %s rate %g\n", (str != NULL)? str : "(null)", rate);
  return 0;
}
