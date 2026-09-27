# corArgs - Command-Line Argument Parser

A type-safe command-line argument parsing library for C with support for multiple data types, validation, constraints, and automatic help generation.

- **Version:** 0.1.0
- **Language:** C
- **License:** [Apache License 2.0](LICENSE)

The only dependency is **kbase**.

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
- **Constraints** - Min/max values for numbers, length limits for strings
- **Automatic help** - Generated from option descriptions
- **Built-in options** - `--usage`, `--verbose`, `--debug` included
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
CorArgFloat // double
CorArgString    // char*
```

### Functions

#### corArgsInit

```c
CorArgsStatus corArgsInit(const char* progName, CorArg* kargV, const char* prefix);
```

Initializes the parser with option definitions. `prefix` is used for environment variables (e.g., "MYAPP_" makes `--port` settable via `MYAPP_PORT`).

#### corArgsParse

```c
CorArgsStatus corArgsParse(int argC, char* argV[]);
```

Parses command-line arguments and populates variables.

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

### Built-in Options

| Option | Short | Description |
|--------|-------|-------------|
| `--usage` | `-u` | Display standard usage |
| `--Usage` | `-U` | Display extended usage |
| `--verbose` | `-v` | Enable verbose mode |
| `--debug` | `-d` | Enable debug mode |

Access via: `corArgsBuiltinUsage`, `corArgsBuiltinVerbose`, `corArgsBuiltinDebug`

## Building

```bash
make          # Build library
make clean    # Remove build artifacts
make install  # Build (nothing to copy: consumers use -I.. and link from this checkout)
```

## Usage Example

```c
#include "corArgs/corArgs.h"

// Variables to hold parsed values
int    port     = 0;
char*  host     = NULL;
bool  verbose  = false;
int    timeout  = 30;

// Option definitions
CorArg options[] = {
    { "--port",    "-p", CorArgInt,    &port,    CorArgReq, _vp 8080, _vp 1, _vp 65535,
      "Server port (1-65535)" },
    { "--host",    "-h", CorArgString, &host,    CorArgOpt, "localhost", NULL, NULL,
      "Server hostname" },
    { "--timeout", "-t", CorArgInt,    &timeout, CorArgOpt, _vp 30, _vp 1, _vp 300,
      "Connection timeout in seconds" },
    { "--verbose", "-v", CorArgBool,   &verbose, CorArgOpt, _vp false, NULL, NULL,
      "Enable verbose output" },
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
    if (verbose)
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

```
Usage: myserver [options]
  --port, -p <int>       Server port (1-65535) [REQUIRED]
  --host, -h <string>    Server hostname [default: localhost]
  --timeout, -t <int>    Connection timeout in seconds [default: 30]
  --verbose, -v          Enable verbose output
  --usage, -u            Display this help
```

## Constraint Validation

```c
// Integer with range
{ "--count", NULL, CorArgInt, &count, CorArgOpt, _vp 10, _vp 1, _vp 100, "Count (1-100)" },

// Unsigned with no upper limit
{ "--size", NULL, CorArgULong, &size, CorArgOpt, _vp 0, _vp 0, CORARGS_NL, "Size in bytes" },

// Float with range
{ "--rate", NULL, CorArgFloat, &rate, CorArgOpt, _vp 1.0, _vp 0.1, _vp 10.0, "Rate (0.1-10.0)" },
```

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

- [kbase](../kbase) - the log macros (KBL_*), K_VEC_SIZE, kStringSort

## License

[Apache 2.0](LICENSE) &copy; 2024-2025 Ken Zangelin
