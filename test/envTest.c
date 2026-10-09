//
// FILE            envTest.c
//
// AUTHOR          Ken Zangelin
//
// Copyright 2026 Seamware
// SPDX-License-Identifier: Apache-2.0
//
// Options set through the environment: corArgsInit(..., prefix) makes --port settable as <prefix>_PORT,
// whether the prefix is given with its trailing underscore ("ENVTEST_") or without it ("ENVTEST").
//
//   envTest <prefix>
//
// For each of the two spellings of the prefix the same variables are set (ENVTEST_PORT, ENVTEST_CONFIG,
// ENVTEST_NO_BROWSER) and the values read back. Exit code 0: all of them arrived.
//
#include <stdio.h>                               // printf
#include <stdlib.h>                              // setenv
#include <string.h>                              // strcmp
#include <stdbool.h>                             // bool

#include "corArgs/corArgs.h"                     // corArgsInit, corArgsParse
#include "corArgs/CorArg.h"                      // CorArg



static unsigned short port;
static char*          config;
static bool           noBrowser;

static CorArg options[] =
{
  { "--port",       "-p",  CorArgUShort, &port,      CorArgOpt, (void*) 7900,  (void*) 1, (void*) 65535, "port" },
  { "--config",     "-c",  CorArgString, &config,    CorArgOpt, NULL,          NULL,      NULL,          "configuration file" },
  { "--no-browser", "-nb", CorArgBool,   &noBrowser, CorArgOpt, (void*) false, NULL,      NULL,          "open no browser" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// main -
//
int main(int argC, char* argV[])
{
  const char* prefix = (argC > 1) ? argV[1] : "ENVTEST_";
  char*       noArgV[] = { "envTest", NULL };

  setenv("ENVTEST_PORT",       "7901",        1);
  setenv("ENVTEST_CONFIG",     "/tmp/c.json", 1);
  setenv("ENVTEST_NO_BROWSER", "true",        1);

  if ((corArgsInit("envTest", options, prefix) != CorArgsOk) || (corArgsParse(1, noArgV) != CorArgsOk))
  {
    printf("prefix '%s': corArgsInit/corArgsParse failed\n", prefix);
    return 1;
  }

  bool ok = (port == 7901) && (config != NULL) && (strcmp(config, "/tmp/c.json") == 0) && (noBrowser == true);

  printf("prefix %-10s %s (port %u, config %s, no-browser %s)\n", prefix, ok ? "OK" : "FAILED",
         port, (config != NULL) ? config : "(null)", noBrowser ? "true" : "false");

  return ok ? 0 : 1;
}
