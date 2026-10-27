# corArgs - Command-Line Argument Parser

A type-safe command-line argument parsing library for C with support for multiple data types, validation, constraints, and automatic help generation.

- **Version:** 0.1.0
- **Language:** C
- **License:** [Apache License 2.0](LICENSE)

The only dependency is **corBase**.

## Where it comes from

corArgs is **kargs** under the cor prefix, copied, not forked: kargs itself is
untouched and keeps serving its own users.

The renames: `kargs*` → `corArgs*` (`kargsInit`, `kargsParse`, `kargsPeek`,
`kargsAdd`, `kargsUsage`, ...), `KArg` → `CorArg`, `KArgsStatus` → `CorArgsStatus`
(values `KargsOk`, ... → `CorArgsOk`, ...), `KargType` → `CorArgType` (values
`KaBool`, `KaInt`, `KaString`, ... → `CorArgBool`, `CorArgInt`, `CorArgString`, ...),
`KargSort` → `CorArgSort` (`KaOpt`/`KaReq`/`KaHid` → `CorArgOpt`/`CorArgReq`/`CorArgHid`),
`KargValueFrom` → `CorArgValueFrom` (`Kavf*` → `CorArgFrom*`), `KARGS_END` →
`CORARGS_END`, `KA_NL` → `CORARGS_NL`, the builtins `kaBuiltinVerbose`/... →
`corArgsBuiltinVerbose`/....

`KBool` became `bool` - and that includes the **variables a `CorArgBool` option
points at**: corArgs stores through `(bool*)`, so such a variable must be a `bool`.

The test cases in `test/` are kargs's own, renamed; five of their expect files
were already stale in kargs (they fail the same way against kargs's own binary).

## Features

- **Type-safe parsing** - Bool, integers (8/16/32/64 bit), float, string
- **Constraints** - min/max values for numbers (whole numbers, a float's too), lexical (`strcmp`) limits for strings
- **Automatic help** - Generated from option descriptions
- **Built-in options** - `--usage`, `--Usage`, `--verbose` included
- **Environment variables** - Options can be set via environment
- **Error collection** - Detailed error reporting with source tracking

## API Reference

### Option Definition

```c
typedef struct CorArg {
    const char*  longName;     // "--port"
    const char*  shortName;    // "-p" (must start with - or +)
    CorArgType     type;       // CorArgBool, CorArgInt, CorArgString, etc.
    void*        valueP;       // Pointer to variable
    CorArgSort     sort;       // CorArgOpt (optional), CorArgReq (required), CorArgHid (hidden)
    void*        def;          // Default value
    void*        min;          // Minimum (CORARGS_NL for no limit)
    void*        max;          // Maximum (CORARGS_NL for no limit)
    const char*  description;  // Help text
} CorArg;
```

### Types

```c
CorArgBool  // Boolean
CorArgChar  // signed char (CorArgInt8)
CorArgUChar // unsigned char (CorArgUInt8)
CorArgShort // short (CorArgInt16)
CorArgUShort    // unsigned short (CorArgUInt16)
CorArgInt   // int (CorArgInt32)
CorArgUInt  // unsigned int (CorArgUInt32)
CorArgLong  // long long (CorArgInt64)
CorArgULong // unsigned long long (CorArgUInt64)
CorArgFloat // float
CorArgString    // char*
```

### Functions

#### corArgsInit

```c
CorArgsStatus corArgsInit(const char* progName, CorArg* kargV, const char* prefix);
```

Initializes the parser with option definitions, and sets the options found in the environment. `prefix` is used for environment variables: "MYAPP" and "MYAPP_" both make `--port` settable via `MYAPP_PORT` (`--no-browser`: `MYAPP_NO_BROWSER`). The name is the option's long name (its short name if it has none), upper-cased, with every character other than a letter, a digit or `_` turned into `_`. The action options - `--usage`, `--Usage` and a program's own `--version` - have none: not listed by `--Usage`, not read. The command line wins over the environment.

A Bool option's variable is on for `true`, `True`, `TRUE`, `on`, `On` or `ON`, off for `false`, `False`, `FALSE`, `off`, `Off` or `OFF`. Any other value (`yes`, `1`, `tRuE`, an empty one) is refused: `<prog>: <VAR>=<value>: not a Bool - ...`. A value the environment holds that is refused - that one, or a number out of range - is not said by `corArgsInit` (nor by `corArgsAdd`): it is kept, and `corArgsParse` says it and returns its status, after the `-u`/`-U` check - so `MYAPP_PORT=0 myapp -u` prints the usage.

#### corArgsParse

```c
CorArgsStatus corArgsParse(int argC, char* argV[]);
```

Parses command-line arguments and populates variables.

- `--usage`/`-u` or `--Usage`/`-U` anywhere on the command line prints that list and exits (`corArgsConfig` sets the exit code, or not to exit) - before any other option is set, so a bad value of another option (`-u -p 0`), on the command line or in the environment, does not stop it.
- A value of the environment refused (see `corArgsInit`): said, and its status returned.
- A required option (`CorArgReq`) set neither on the command line nor in the environment: `<prog>: missing required option: <name>`, the usage, exit 1. An action option is never required.

#### corArgsUsage / corArgsExtUsage

```c
void corArgsUsage(void);    // Standard usage
void corArgsExtUsage(void); // Extended usage with more detail
```

#### corArgsErrorGet

```c
CorArgsError* corArgsErrorGet(void);
```

Returns linked list of parsing errors.

#### corArgsPeek

```c
char* corArgsPeek(int argC, char* argV[], CorArg* kargV, char* optName);
```

Inspects arguments without full initialization. Useful for early checks.

#### corArgsSecret

```c
bool corArgsSecret(const char* longName);
```

Marks an option, after it is declared (`corArgsInit`, `corArgsAdd`), as one whose value is never shown - a
password, a URI that may hold credentials: `CorArgInfo.secret`. A tool listing the options from
`corArgInfoV` (coraine's `GET /admin/config`) masks it. `false`: no such option.

### Built-in Options

| Option | Short | Description |
|--------|-------|-------------|
| `--usage` | `-u` | Display standard usage |
| `--Usage` | `-U` | Display extended usage: one row per option - long and short name, environment variable, sort, type and default value (`-` for none; a secret option's is not shown) |
| `--verbose` | `-v` | Enable verbose mode |

Access via: `corArgsBuiltinUsage`, `corArgsBuiltinExtUsage`, `corArgsBuiltinVerbose`. There is no `--debug`: what a debug switch would show, the trace levels (`corLog`) do.

## Building

```bash
make          # Build library
make clean    # Remove build artifacts
make install  # Build (nothing to copy: consumers use -I.. and link from this checkout)
make envTest  # options set through the environment
make limitsTest  # a string's and a float's limits; corArgsInit refusing a table (a default outside a limit, a name twice); a section heading
make parseTest  # a required option, -u/-U beside a bad value, the action options' environment variables, a group of switches, a Bool's environment values
```

## Usage Example

```c
#include "corArgs/corArgs.h"

// Variables to hold parsed values
int    port     = 0;
char*  host     = NULL;
int    timeout  = 30;

// Option definitions - --verbose/-v is a builtin (corArgsBuiltinVerbose): declaring it again is CorArgsNameTaken
CorArg options[] = {
    { "--port",    "-p", CorArgInt,    &port,    CorArgReq, _vp 8080, _vp 1, _vp 65535,
      "Server port (1-65535)" },
    { "--host",    "-h", CorArgString, &host,    CorArgOpt, "localhost", NULL, NULL,
      "Server hostname" },
    CORARGS_SEPARATOR("Tuning"),
    { "--timeout", "-t", CorArgInt,    &timeout, CorArgOpt, _vp 30, _vp 1, _vp 300,
      "Connection timeout in seconds" },
    CORARGS_END
};

int main(int argc, char* argv[])
{
    // Initialize
    if (corArgsInit("myserver", options, "MYSERVER_") != CorArgsOk) {
        fprintf(stderr, "Failed to initialize argument parser\n");
        return 1;
    }

    // Parse
    if (corArgsParse(argc, argv) != CorArgsOk) {
        CorArgsError* err = corArgsErrorGet();
        while (err != NULL) {
            fprintf(stderr, "Error: %s - %s\n", err->optionName, err->description);
            err = err->next;
        }
        corArgsUsage();
        return 1;
    }

    // Use parsed values
    printf("Starting server on %s:%d (timeout: %ds)\n", host, port, timeout);
    if (corArgsBuiltinVerbose)
        printf("Verbose mode enabled\n");

    return 0;
}
```

### Command Line

```bash
# Using long options
./myserver --port 9000 --host example.com --verbose

# Using short options
./myserver -p 9000 -h example.com -v

# Using environment variables
MYSERVER_PORT=9000 ./myserver

# Show help
./myserver --usage
```

### Generated Usage Output

`./myserver -u` - an optional option in brackets, a `CORARGS_SEPARATOR` as its title:

```
Usage: myserver  [--usage/-u (usage)]
                 [--Usage/-U (extended usage)]
                 [--verbose/-v (verbose mode)]
                 --port/-p <Server port (1-65535)>
                 [--host/-h <Server hostname>]

                 Tuning
                 [--timeout/-t <Connection timeout in seconds>]
```

`./myserver -U` - one row per option: names, environment variable, sort, type, default:

```
  --usage   (-u)                  Optional   Bool     false
  --Usage   (-U)                  Optional   Bool     false
  --verbose (-v) MYSERVER_VERBOSE Optional   Bool     false
  --port    (-p) MYSERVER_PORT    Required   Int      8080
  --host    (-h) MYSERVER_HOST    Optional   String   localhost
  --timeout (-t) MYSERVER_TIMEOUT Optional   Int      30
```

The usage, and every error about a value (`<prog>: value ... for option ...`), go to **stdout**.

## Constraint Validation

```c
// Integer with range
{ "--count", NULL, CorArgInt, &count, CorArgOpt, _vp 10, _vp 1, _vp 100, "Count (1-100)" },

// Unsigned with no upper limit
{ "--size", NULL, CorArgULong, &size, CorArgOpt, _vp 0, _vp 0, CORARGS_NL, "Size in bytes" },

// Float with range - a float's default and limits are whole numbers: the void* holds a long long
{ "--rate", NULL, CorArgFloat, &rate, CorArgOpt, _vp 1, _vp 1, _vp 10, "Rate (1-10)" },

// String with lexical limits (strcmp): '050' to '100'. NULL, like CORARGS_NL, is no limit for a string
{ "--code", NULL, CorArgString, &code, CorArgOpt, "060", _vp "050", _vp "100", "Code ('050'-'100')" },
```

A value outside its limits is refused - on the command line and in the environment alike - with the
limit or the range named. `corArgsInit` refuses a table (`CorArgsBadParam`, `CorArgsNameTaken`, the
option named in `corArgsErrorGet()`'s last item) with:

- an option without a name, or a name used twice - by two options, or by an option and a builtin;
- a limit or a default outside its type's range, a min above the max, a default outside its limits
  (one limit alone too; an option without a default, `CORARGS_NL`, is not checked);
- a NULL `valueP`, an invalid type or sort, a NULL description.

`CORARGS_SEPARATOR("title")` - a section heading in the usage - has no name, variable or limits,
and goes in the table given to `corArgsInit` or `corArgsAdd`.

## Error Handling

```c
CorArgsStatus status = corArgsParse(argc, argv);
if (status != CorArgsOk) {
    CorArgsError* err = corArgsErrorGet();
    while (err != NULL) {
        fprintf(stderr, "%s[%d] %s: %s - %s\n",
                err->fileName, err->lineNo, err->funcName,
                err->optionName, err->description);
        err = err->next;
    }
}
```

## Dependencies

- [corBase](../corBase) - the library log (COR_LIB_*), COR_VEC_SIZE, corStringSort

## License

[Apache 2.0](LICENSE) &copy; 2024-2025 Ken Zangelin
