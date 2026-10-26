//
// FILE            parseTest.c
//
// AUTHOR          Ken Zangelin
//
// Copyright 2026 Seamware
// SPDX-License-Identifier: Apache-2.0
//
// The program test/parseTest.sh runs with different command lines and environments: a required option
// (--name), an option with limits (--port, 1-65535) and an action option of its own (--version), the
// environment prefix PARSETEST.
//
//   parseTest [options]
//
// Once parsed, one line: "OK name <name> port <port> version <true|false>", exit code 0. Everything
// else - a missing required option, a bad value, the usage - is corArgs's own output and exit code.
//
#include <stdio.h>                               // printf
#include <stdbool.h>                             // bool

#include "corArgs/corArgs.h"                     // corArgsInit, corArgsParse
#include "corArgs/CorArg.h"                      // CorArg



static char*          name;
static unsigned short port;
static bool           version;
static char*          dir;
static bool           quiet;

static CorArg options[] =
{
  { "--name",    "-n", CorArgString, &name,    CorArgReq, NULL,          NULL,      NULL,          "a name" },
  { "--port",    "-p", CorArgUShort, &port,    CorArgOpt, (void*) 7900,  (void*) 1, (void*) 65535, "port" },
  { "--version", "-V", CorArgBool,   &version, CorArgOpt, (void*) false, NULL,      NULL,          "print version and exit" },
  { "--dir",     "-dir", CorArgString, &dir,   CorArgOpt, NULL,          NULL,      NULL,          "a directory" },
  { "--quiet",   "-q", CorArgBool,   &quiet,   CorArgOpt, (void*) false, NULL,      NULL,          "say less" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// main -
//
int main(int argC, char* argV[])
{
  if (corArgsInit("parseTest", options, "PARSETEST") != CorArgsOk)
  {
    printf("corArgsInit failed\n");
    return 2;
  }

  if (corArgsParse(argC, argV) != CorArgsOk)
    return 3;

  printf("OK name %s port %u version %s\n", (name != NULL) ? name : "(null)", port, version ? "true" : "false");
  if (dir != NULL)
    printf("dir %s\n", dir);
  if (quiet == true)
    printf("quiet\n");
  return 0;
}
