// -----------------------------------------------------------------------------
//
// FILE                  corArgsTest.c - test program for the corArgs library
//
// AUTHOR                Ken Zangelin
//
// Copyright 2024 Ken Zangelin
//
// SPDX-License-Identifier: Apache-2.0
//
#include <stdio.h>                     // printf, NULL
#include <stdarg.h>                    // va_list
#include <stdbool.h>                   // bool
#include <string.h>                    // strcmp

#include "corBase/corMacros.h"         // COR_FT
#include "corBase/corLibLog.h"         // COR_LIB_*
#include "corBase/corBaseInit.h"       // corBaseInit

#include "corArgs/corArgs.h"           // corArgs library



// -----------------------------------------------------------------------------
//
// Variables to hold CLI parameters
//
bool                ktest;
char                cMin;
char                cMax;
char                cMinMax;
char                charVal;
short               sMin;
short               sMax;
short               sMinMax;
short               shortVal;
int                 iMin;
int                 iMax;
int                 iMinMax;
int                 intVal;
long long           llMin;
long long           llMax;
long long           llMinMax;
long long           longVal;
unsigned char       ucMin;
unsigned char       ucMax;
unsigned char       ucMinMax;
unsigned char       ucharVal;
unsigned short      usMin;
unsigned short      usMax;
unsigned short      usMinMax;
unsigned short      ushortVal;
unsigned int        uiMin;
unsigned int        uiMax;
unsigned int        uiMinMax;
unsigned int        uintVal;
unsigned long long  ullMin;
unsigned long long  ullMax;
unsigned long long  ullMinMax;
unsigned long long  ulongVal;
float               fMin;
float               fMax;
float               fMinMax;
char*               strMin;
char*               strMax;
char*               strMinMax;
bool                b1;
bool                b2;
bool                b3;
bool                peek;
char*               speek;
char*               testCase;



// -----------------------------------------------------------------------------
//
// kargs - vector of CLI parameters for kargs
//
CorArg kargs[] =
{
  { "--ktest",     "-k", CorArgBool,   &ktest,     CorArgHid, _vp false,    _vp false, _vp true, "running under ktest test-suite"     },
  { "--case",      NULL, CorArgString, &testCase,  CorArgHid, NULL,    CORARGS_NL,     CORARGS_NL,    "Test Case"                        },

  { "--b1",        "-1", CorArgBool,   &b1,        CorArgOpt, _vp false,    _vp false, _vp true, "Bool 1"                             },
  { "--b2",        "-2", CorArgBool,   &b2,        CorArgOpt, _vp false,    _vp false, _vp true, "Bool 2"                             },
  { "--b3",        "-3", CorArgBool,   &b3,        CorArgOpt, _vp false,    _vp false, _vp true, "Bool 3"                             },
  { "--peek",      "-p", CorArgBool,   &peek,      CorArgHid, _vp false,    _vp false, _vp true, "Peek"                               },
  { "--speek",     "-s", CorArgString, &speek, CorArgHid, NULL,    CORARGS_NL,     CORARGS_NL,    "String Peek"                      },

  { "--cMin",      NULL, CorArgChar,   &cMin,      CorArgOpt, _vp 1,    _vp -5,     CORARGS_NL,    "char with min limit -5"           },
  { "--cMax",      NULL, CorArgChar,   &cMax,      CorArgOpt, _vp 1,    CORARGS_NL,     _vp 10,    "char with max limit 10"           },
  { "--cMinMax",   NULL, CorArgChar,   &cMinMax,   CorArgOpt, _vp 1,    _vp -5,     _vp 10,    "char with limits -5-10"           },
  { "--charVal",   NULL, CorArgChar,   &charVal,   CorArgOpt, _vp 1,    CORARGS_NL,     CORARGS_NL,    "char without limits"              },

  { "--sMin",      NULL, CorArgShort,  &sMin,      CorArgOpt, _vp 2,    _vp -5,      CORARGS_NL,   "short with min limit -5"          },
  { "--sMax",      NULL, CorArgShort,  &sMax,      CorArgOpt, _vp 2,    CORARGS_NL,     _vp 10,    "short with max limit 10"          },
  { "--sMinMax",   NULL, CorArgShort,  &sMinMax,   CorArgOpt, _vp 2,    _vp -5,      _vp 10,   "short with limits -5-10"          },
  { "--shortVal",  NULL, CorArgShort,  &shortVal,  CorArgOpt, _vp 2,    CORARGS_NL,     CORARGS_NL,    "short without limits"             },

  { "--iMin",      NULL, CorArgInt,    &iMin,      CorArgOpt, _vp 3,    _vp -5,      CORARGS_NL,   "int with min limit -5"            },
  { "--iMax",      NULL, CorArgInt,    &iMax,      CorArgOpt, _vp 3,    CORARGS_NL,     _vp 10,    "int with max limit 10"            },
  { "--iMinMax",   NULL, CorArgInt,    &iMinMax,   CorArgOpt, _vp 3,    _vp -5,      _vp 10,   "int with limits -5-10"            },
  { "--intVal",    NULL, CorArgInt,    &intVal,    CorArgOpt, _vp 3,    CORARGS_NL,     CORARGS_NL,    "int without limits"               },

  { "--llMin",     NULL, CorArgLong,   &llMin,     CorArgOpt, _vp 4,    _vp -5,      CORARGS_NL,   "long with min limit -5"           },
  { "--llMax",     NULL, CorArgLong,   &llMax,     CorArgOpt, _vp 4,    CORARGS_NL,     _vp 10,    "long with max limit 10"           },
  { "--llMinMax",  NULL, CorArgLong,   &llMinMax,  CorArgOpt, _vp 4,    _vp -5,      _vp 10,   "long with limits -5-10"           },
  { "--longVal",   NULL, CorArgLong,   &longVal,   CorArgOpt, _vp 4,    CORARGS_NL, CORARGS_NL,   "long without limits"              },

  { "--ucMin",     NULL, CorArgUChar,  &ucMin,     CorArgOpt, _vp 5,    _vp 5,      CORARGS_NL,    "unsigned char with min limit 5"   },
  { "--ucMax",     NULL, CorArgUChar,  &ucMax,     CorArgOpt, _vp 5,    CORARGS_NL,     _vp 10,    "unsigned char with max limit 10"  },
  { "--ucMinMax",  NULL, CorArgUChar,  &ucMinMax,  CorArgOpt, _vp 5,    _vp 5,     _vp 10,     "unsigned char with limits 5-10"   },

  { "--usMin",     NULL, CorArgUShort, &usMin, CorArgOpt, _vp 6,    _vp 5,      CORARGS_NL,    "unsigned short with min limit 5"  },
  { "--usMax",     NULL, CorArgUShort, &usMax, CorArgOpt, _vp 6,    CORARGS_NL,     _vp 10,    "unsigned short with max limit 10" },
  { "--usMinMax",  NULL, CorArgUShort, &usMinMax,  CorArgOpt, _vp 6,    _vp 5,      _vp 10,    "unsigned short with limits 5-10"  },

  { "--uiMin",     NULL, CorArgUInt,   &uiMin,     CorArgOpt, _vp 7,    _vp 5,      CORARGS_NL,    "unsigned int with min limit 5"    },
  { "--uiMax",     NULL, CorArgUInt,   &uiMax,     CorArgOpt, _vp 7,    CORARGS_NL,     _vp 10,    "unsigned int with max limit 10"   },
  { "--uiMinMax",  NULL, CorArgUInt,   &uiMinMax,  CorArgOpt, _vp 7,    _vp 5,      _vp 10,    "unsigned int with limits 5-10"    },

  { "--ullMin",    NULL, CorArgULong,  &ullMin,    CorArgOpt, _vp 8,    _vp 5,      CORARGS_NL,    "unsigned long with min limit 5"   },
  { "--ullMax",    NULL, CorArgULong,  &ullMax,    CorArgOpt, _vp 8,    CORARGS_NL,     _vp 10,    "unsigned long with max limit 10"  },
  { "--ullMinMax", NULL, CorArgULong,  &ullMinMax, CorArgOpt, _vp 8,    _vp 5,      _vp 10,    "unsigned ling with limits 5-10"   },

  { "--fMin",      NULL, CorArgFloat,  &fMin,      CorArgOpt, _vp 55,   _vp 50,     CORARGS_NL,    "float with min limit 50"          },
  { "--fMax",      NULL, CorArgFloat,  &fMax,      CorArgOpt, _vp 56,   CORARGS_NL,     _vp 100,   "float with max limit 100"         },
  { "--fMinMax",   NULL, CorArgFloat,  &fMinMax,   CorArgOpt, _vp 57,   _vp 50,     _vp 100,   "float with limits 50-100"         },

  { "--strMin",    NULL, CorArgString, &strMin,    CorArgOpt, "060",    _vp "050",  CORARGS_NL,    "string with min limit '050'"     },
  { "--strMax",    NULL, CorArgString, &strMax,    CorArgOpt, "060",    CORARGS_NL,     _vp "100", "string with max limit '100'"     },
  { "--strMinMax", NULL, CorArgString, &strMinMax, CorArgOpt, "060",    _vp "050",  _vp "100", "string with limits '050'-'100'"  },

  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// Tests for errors in the CorArg vector:
// - min value out of bounds for CorArgChar    (sMin < MIN_CHAR)     corArgsMinCharOutOfBounds                 01
// - min value out of bounds for CorArgUChar   (sMin is negative)    corArgsMinUCharOutOfBounds                02
// - min value out of bounds for CorArgShort   (sMin < MIN_SHORT)    corArgsMinShortOutOfBounds                03
// - min value out of bounds for CorArgUShort  (sMin is negative)    corArgsMinUShortOutOfBounds               04
// - min value out of bounds for CorArgInt (sMin < MIN_INT)      corArgsMinIntOutOfBounds                  05
// - min value out of bounds for CorArgUInt    (sMin is negative)    corArgsMinUIntOutOfBounds                 06
//
// - max value out of bounds for CorArgChar    (sMax > MAX_CHAR)     corArgsMaxCharOutOfBounds                 07
// - max value out of bounds for CorArgUChar   (usMax > MAX_UCHAR)   corArgsMaxUCharOutOfBounds                08
// - max value out of bounds for CorArgShort   (sMax > MAX_SHORT)    corArgsMaxShortOutOfBounds                09
// - max value out of bounds for CorArgUShort  (usMax > MAX_USHORT)  corArgsMaxUShortOutOfBounds               10
// - max value out of bounds for CorArgInt (sMax > MAX_INT)      corArgsMaxIntOutOfBounds                  11
// - max value out of bounds for CorArgUInt    (usMax > MAX_UINT)    corArgsMaxUIntOutOfBounds                 12
//
// - def value out of bounds for CorArgChar    (sDef > MAX_CHAR)     corArgsDefCharOutOfMaxBounds              13
// - def value out of bounds for CorArgChar    (sDef < MIN_CHAR)     corArgsDefCharOutOfMinBounds              14
// - def value out of bounds for CorArgUChar   (usDef > MAX_UCHAR)   corArgsDefUCharOutOfMaxBounds             15
// - def value out of bounds for CorArgUChar   (sDef is negative)    corArgsDefUCharOutOfMinBounds             16
// - def value out of bounds for CorArgShort   (sDef > MAX_SHORT)    corArgsDefShortOutOfMaxBounds             17
// - def value out of bounds for CorArgShort   (sDef < MIN_SHORT)    corArgsDefShortOutOfMinBounds             18
// - def value out of bounds for CorArgUShort  (usDef > MAX_USHORT)  corArgsDefUShortOutOfMaxBounds            19
// - def value out of bounds for CorArgUShort  (sDef is negative)    corArgsDefUShortOutOfMinBounds            20
// - def value out of bounds for CorArgInt (sDef > MAX_INT)      corArgsDefIntOutOfMaxBounds               21
// - def value out of bounds for CorArgInt (sDef < MIN_INT)      corArgsDefIntOutOfMinBounds               22
// - def value out of bounds for CorArgUInt    (usDef > MAX_UINT)    corArgsDefUIntOutOfMaxBounds              23
// - def value out of bounds for CorArgUInt    (sDef is negative)    corArgsDefUIntOutOfMinBounds              24
//
// - char max value < min value                                  corArgsCharMaxLimitLessThanMinLimit       25
// - uchar max value < min value                                 corArgsUCharMaxLimitLessThanMinLimit      26
// - short max value < min value                                 corArgsShortMaxLimitLessThanMinLimit      27
// - ushort max value < min value                                corArgsUShortMaxLimitLessThanMinLimit     28
// - int max value < min value                                   corArgsIntMaxLimitLessThanMinLimit        29
// - uint max value < min value                                  corArgsUIntMaxLimitLessThanMinLimit       30
// - long max value < min value                                  corArgsLongMaxLimitLessThanMinLimit       31
// - ulong max value < min value                                 corArgsULongMaxLimitLessThanMinLimit      32
// - char def value < min value                                  corArgsCharDefValueLessThanMinLimit       33
// - uchar def value < min value                                 corArgsUCharDefValueLessThanMinLimit      34
// - short def value < min value                                 corArgsShortDefValueLessThanMinLimit      35
// - ushort def value < min value                                corArgsUShortDefValueLessThanMinLimit     36
// - int def value < min value                                   corArgsIntDefValueLessThanMinLimit        37
// - uint def value < min value                                  corArgsUIntDefValueLessThanMinLimit       38
// - long def value < min value                                  corArgsLongDefValueLessThanMinLimit       39
// - ulong def value < min value                                 corArgsULongDefValueLessThanMinLimit      40
// - char def value > max value                                  corArgsCharDefValueExceedsMaxLimit        41
// - uchar def value > max value                                 corArgsUCharDefValueExceedsMaxLimit       42
// - short def value > max value                                 corArgsShortDefValueExceedsMaxLimit       43
// - ushort def value > max value                                corArgsUShortDefValueExceedsMaxLimit      44
// - int def value > max value                                   corArgsIntDefValueExceedsMaxLimit         45
// - uint def value > max value                                  corArgsUIntDefValueExceedsMaxLimit        46
// - long def value > max value                                  corArgsLongDefValueExceedsMaxLimit        47
// - ulong def value > max value                                 corArgsULongDefValueExceedsMaxLimit       48
//
// - No longName nor shortName                                   corArgsNoOptionName                       49
// - Invalid CorArgType                                          corArgsInvalidType                        50
// - Invalid CorArgSort                                          corArgsInvalidSort                        51
// - NULL valueP                                                 corArgsNullValue                          52
// - NULL description                                            corArgsNullDescription                    53
//
// - option short-name taken by builtin short-name               corArgsShortNameTakenByBuiltinShortName   54
// - option short-name taken by builtin long-name                corArgsShortNameTakenByBuiltinLongName    55
// - option long-name taken by builtin short-name                corArgsLongNameTakenByBuiltinShortName    56
// - option long-name taken by builtin long-name                 corArgsLongNameTakenByBuiltinLongName     57
//
// - option short-name taken by other option short-name          corArgsShortNameTakenByOptionShortName    58
// - option short-name taken by other option long-name           corArgsShortNameTakenByOptionLongName     59
// - option long-name taken by other option short-name           corArgsLongNameTakenByOptionShortName     60
// - option long-name taken by other option long-name            corArgsLongNameTakenByOptionLongName      61
//
// - option long name == same option short name                  corArgsShortLongNameIsTheSame             62
//
//


// -----------------------------------------------------------------------------
//
// corArgsMinCharOutOfBounds (01) - min value out of bounds for CorArgChar (sMin < MIN_CHAR)
//
CorArg corArgsMinCharOutOfBounds[] =
{
  { "--MinCharOutOfBounds", "-m", CorArgChar, &charVal, CorArgOpt, _vp 0, _vp -129, _vp 10, "--MinCharOutOfBounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsMinUCharOutOfBounds (02) - min value out of bounds for CorArgUChar (sMin is negative)
//
CorArg corArgsMinUCharOutOfBounds[] =
{
  { "--MinUCharOutOfBounds", "-m", CorArgUChar, &ucharVal, CorArgOpt, _vp 0, _vp -1, _vp 10, "--MinUCharOutOfBounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsMinShortOutOfBounds (03) - min value out of bounds for CorArgShort (sMin < MIN_SHORT)
//
CorArg corArgsMinShortOutOfBounds[] =
{
  { "--MinShortOutOfBounds", "-m", CorArgShort, &shortVal, CorArgOpt, _vp 0, _vp -100000, _vp 10, "--MinShortOutOfBounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsMinUShortOutOfBounds (04) - min value out of bounds for CorArgUShort (sMin is negative)
//
CorArg corArgsMinUShortOutOfBounds[] =
{
  { "--MinUShortOutOfBounds", "-m", CorArgUShort, &shortVal, CorArgOpt, _vp 0, _vp -1, _vp 10, "--MinUShortOutOfBounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsMinIntOutOfBounds (05) - min value out of bounds for CorArgInt (sMin < MIN_INT)
//
CorArg corArgsMinIntOutOfBounds[] =
{
  { "--MinIntOutOfBounds", "-m", CorArgInt, &intVal, CorArgOpt, _vp -2147483649, _vp -1, _vp 10, "--MinIntOutOfBounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsMinUIntOutOfBounds (06) - min value out of bounds for CorArgUInt (sMin is negative)
//
CorArg corArgsMinUIntOutOfBounds[] =
{
  { "--MinUIntOutOfBounds", "-m", CorArgUInt, &intVal, CorArgOpt, _vp 1, _vp -1, _vp 10, "--MinUIntOutOfBounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsMaxCharOutOfBounds (07) - max value out of bounds for CorArgChar (sMax > MAX_CHAR) 
//
CorArg corArgsMaxCharOutOfBounds[] =
{
  { "--MaxCharOutOfBounds", "-m", CorArgChar, &charVal, CorArgOpt, _vp 1, _vp 0, _vp 128, "--MaxCharOutOfBounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsMaxUCharOutOfBounds (08) - max value out of bounds for CorArgUChar (usMax > MAX_UCHAR)
//
CorArg corArgsMaxUCharOutOfBounds[] =
{
  { "--MaxUCharOutOfBounds", "-m", CorArgUChar, &ucharVal, CorArgOpt, _vp 1, _vp 0, _vp 256, "--MaxUCharOutOfBounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsMaxShortOutOfBounds (09) - max value out of bounds for CorArgShort (sMax > MAX_SHORT)
//
CorArg corArgsMaxShortOutOfBounds[] =
{
  { "--MaxShortOutOfBounds", "-m", CorArgShort, &shortVal, CorArgOpt, _vp 1, _vp 0, _vp 40000, "--MaxShortOutOfBounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsMaxUShortOutOfBounds (10) - max value out of bounds for CorArgUShort (usMax > MAX_USHORT)
//
CorArg corArgsMaxUShortOutOfBounds[] =
{
  { "--MaxUShortOutOfBounds", "-m", CorArgUShort, &ushortVal, CorArgOpt, _vp 1, _vp 0, _vp 66000, "--MaxUShortOutOfBounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsMaxIntOutOfBounds (11) - max value out of bounds for CorArgInt (sMax > MAX_INT)
//
CorArg corArgsMaxIntOutOfBounds[] =
{
  { "--MaxIntOutOfBounds", "-m", CorArgInt, &intVal, CorArgOpt, _vp 1, _vp 0, _vp 0xFFFFFFFFF, "--MaxIntOutOfBounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsMaxUIntOutOfBounds (12) - max value out of bounds for CorArgUInt (usMax > MAX_UINT)
//
CorArg corArgsMaxUIntOutOfBounds[] =
{
  { "--MaxUIntOutOfBounds", "-m", CorArgUInt, &uintVal, CorArgOpt, _vp 1, _vp 0, _vp 0xFFFFFFFFF, "--MaxUIntOutOfBounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsDefCharOutOfMaxBounds (13) - def value out of bounds for CorArgChar (sDef > MAX_CHAR)
//
CorArg corArgsDefCharOutOfMaxBounds[] =
{
  { "--DefCharOutOfMaxBounds", NULL, CorArgChar, &charVal, CorArgOpt, _vp 128, CORARGS_NL, CORARGS_NL, "default value out of bounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsDefCharOutOfMinBounds (14) - def value out of bounds for CorArgChar (sDef < MIN_CHAR)
//
CorArg corArgsDefCharOutOfMinBounds[] =
{
  { "--DefCharOutOfMinBounds", NULL, CorArgChar, &charVal, CorArgOpt, _vp -129, CORARGS_NL, CORARGS_NL, "default value out of bounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsDefUCharOutOfMaxBounds (15) - def value out of bounds for CorArgUChar (usDef > MAX_UCHAR)
//
CorArg corArgsDefUCharOutOfMaxBounds[] =
{
  { "--DefUCharOutOfMaxBounds", NULL, CorArgUChar, &ucharVal, CorArgOpt, _vp 256, CORARGS_NL, CORARGS_NL, "default value out of bounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsDefUCharOutOfMinBounds (16) - def value out of bounds for CorArgUChar (sDef is negative)
//
CorArg corArgsDefUCharOutOfMinBounds[] =
{
  { "--DefUCharOutOfMinBounds", NULL, CorArgUChar, &ucharVal, CorArgOpt, _vp -1, CORARGS_NL, CORARGS_NL, "default value out of bounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsDefShortOutOfMaxBounds (17) - def value out of bounds for CorArgShort (sDef > MAX_SHORT)
//
CorArg corArgsDefShortOutOfMaxBounds[] =
{
  { "--DefShortOutOfMaxBounds", NULL, CorArgShort, &shortVal, CorArgOpt, _vp 36000, CORARGS_NL, CORARGS_NL, "default value out of bounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsDefShortOutOfMinBounds END(18) - def value out of bounds for CorArgShort (sDef < MIN_SHORT)
//
CorArg corArgsDefShortOutOfMinBounds[] =
{
  { "--DefShortOutOfMinBounds", NULL, CorArgShort, &shortVal, CorArgOpt, _vp -36000, CORARGS_NL, CORARGS_NL, "default value out of bounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsDefUShortOutOfMaxBounds (19) - def value out of bounds for CorArgUShort (usDef > MAX_USHORT)
//
CorArg corArgsDefUShortOutOfMaxBounds[] =
{
  { "--DefUShortOutOfMaxBounds", NULL, CorArgUShort, &ushortVal, CorArgOpt, _vp 66000, CORARGS_NL, CORARGS_NL, "default value out of bounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsDefUShortOutOfMinBounds (20) - def value out of bounds for CorArgUShort (sDef is negative)
//
CorArg corArgsDefUShortOutOfMinBounds[] =
{
  { "--DefUShortOutOfMinBounds", NULL, CorArgUShort, &ushortVal, CorArgOpt, _vp -1, CORARGS_NL, CORARGS_NL, "default value out of bounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsDefIntOutOfMaxBounds (21) - def value out of bounds for CorArgInt (sDef > MAX_INT)
//
CorArg corArgsDefIntOutOfMaxBounds[] =
{
  { "--DefIntOutOfMaxBounds", NULL, CorArgInt, &intVal, CorArgOpt, _vp 3000000000, CORARGS_NL, CORARGS_NL, "default value out of bounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsDefIntOutOfMinBounds (22) - def value out of bounds for CorArgInt (sDef < MIN_INT)
//
CorArg corArgsDefIntOutOfMinBounds[] =
{
  { "--DefIntOutOfMinBounds", NULL, CorArgInt, &intVal, CorArgOpt, _vp -3000000000, CORARGS_NL, CORARGS_NL, "default value out of bounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsDefUIntOutOfMaxBounds (23) - def value out of bounds for CorArgUInt (usDef > MAX_UINT)
//
CorArg corArgsDefUIntOutOfMaxBounds[] =
{
  { "--DefUIntOutOfMaxBounds", NULL, CorArgUInt, &uintVal, CorArgOpt, _vp 5000000000, CORARGS_NL, CORARGS_NL, "default value out of bounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsDefUIntOutOfMinBounds (24) - def value out of bounds for CorArgUInt (sDef is negative)
//
CorArg corArgsDefUIntOutOfMinBounds[] =
{
  { "--DefUIntOutOfMinBounds", NULL, CorArgUInt, &uintVal, CorArgOpt, _vp -1, CORARGS_NL, CORARGS_NL, "default value out of bounds" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsCharMaxLimitLessThanMinLimit (25) - char max value < min value                                
//
CorArg corArgsCharMaxLimitLessThanMinLimit[] =
{
  { "--CharMaxLimitLessThanMinLimit", NULL, CorArgChar, &charVal, CorArgOpt, _vp 22, _vp 25, _vp 20, "CharMaxLimitLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsUCharMaxLimitLessThanMinLimit (26) - uchar max value < min value
//
CorArg corArgsUCharMaxLimitLessThanMinLimit[] =
{
  { "--UCharMaxLimitLessThanMinLimit", NULL, CorArgUChar, &ucharVal, CorArgOpt, _vp 22, _vp 25, _vp 20, "UCharMaxLimitLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsShortMaxLimitLessThanMinLimit (27) - short max value < min value                               
//
CorArg corArgsShortMaxLimitLessThanMinLimit[] =
{
  { "--ShortMaxLimitLessThanMinLimit", NULL, CorArgShort, &shortVal, CorArgOpt, _vp 22, _vp 25, _vp 20, "ShortMaxLimitLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsUShortMaxLimitLessThanMinLimit (28) - ushort max value < min value                              
//
CorArg corArgsUShortMaxLimitLessThanMinLimit[] =
{
  { "--UShortMaxLimitLessThanMinLimit", NULL, CorArgUShort, &ushortVal, CorArgOpt, _vp 22, _vp 25, _vp 20, "UShortMaxLimitLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsIntMaxLimitLessThanMinLimit (29) - int max value < min value                                 
//
CorArg corArgsIntMaxLimitLessThanMinLimit[] =
{
  { "--IntMaxLimitLessThanMinLimit", NULL, CorArgInt, &intVal, CorArgOpt, _vp 22, _vp 25, _vp 20, "IntMaxLimitLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsUIntMaxLimitLessThanMinLimit (30) - uint max value < min value                                
//
CorArg corArgsUIntMaxLimitLessThanMinLimit[] =
{
  { "--UIntMaxLimitLessThanMinLimit", NULL, CorArgUInt, &uintVal, CorArgOpt, _vp 22, _vp 25, _vp 20, "UIntMaxLimitLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsLongMaxLimitLessThanMinLimit (31) - long max value < min value
//
CorArg corArgsLongMaxLimitLessThanMinLimit[] =
{
  { "--LongMaxLimitLessThanMinLimit", NULL, CorArgLong, &longVal, CorArgOpt, _vp 22, _vp 25, _vp 20, "LongMaxLimitLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsULongMaxLimitLessThanMinLimit (32) - ulong max value < min value
//
CorArg corArgsULongMaxLimitLessThanMinLimit[] =
{
  { "--ULongMaxLimitLessThanMinLimit", NULL, CorArgULong, &ulongVal, CorArgOpt, _vp 22, _vp 25, _vp 20, "ULongMaxLimitLessThanMinLimit" },
  CORARGS_END
};




// -----------------------------------------------------------------------------
//
// corArgsCharDefValueLessThanMinLimit (33) - char def value < min value
//
CorArg corArgsCharDefValueLessThanMinLimit[] =
{
  { "--CharDefValueLessThanMinLimit", NULL, CorArgChar, &charVal, CorArgOpt, _vp 9, _vp 10, _vp 20, "CharDefValueLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsUCharDefValueLessThanMinLimit (34) - uchar def value < min value                               
//
CorArg corArgsUCharDefValueLessThanMinLimit[] =
{
  { "--UCharDefValueLessThanMinLimit", NULL, CorArgUChar, &ucharVal, CorArgOpt, _vp 9, _vp 10, _vp 20, "UCharDefValueLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsShortDefValueLessThanMinLimit (35) - short def value < min value                               
//
CorArg corArgsShortDefValueLessThanMinLimit[] =
{
  { "--ShortDefValueLessThanMinLimit", NULL, CorArgShort, &shortVal, CorArgOpt, _vp 9, _vp 10, _vp 20, "ShortDefValueLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsUShortDefValueLessThanMinLimit (36) - ushort def value < min value                              
//
CorArg corArgsUShortDefValueLessThanMinLimit[] =
{
  { "--UShortDefValueLessThanMinLimit", NULL, CorArgUShort, &ushortVal, CorArgOpt, _vp 9, _vp 10, _vp 20, "UShortDefValueLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsIntDefValueLessThanMinLimit (37) - int def value < min value                                 
//
CorArg corArgsIntDefValueLessThanMinLimit[] =
{
  { "--IntDefValueLessThanMinLimit", NULL, CorArgInt, &intVal, CorArgOpt, _vp 9, _vp 10, _vp 20, "IntDefValueLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsUIntDefValueLessThanMinLimit (38) - uint def value < min value
//
CorArg corArgsUIntDefValueLessThanMinLimit[] =
{
  { "--UIntDefValueLessThanMinLimit", NULL, CorArgUInt, &uintVal, CorArgOpt, _vp 9, _vp 10, _vp 20, "UIntDefValueLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsLongDefValueLessThanMinLimit (39) - long def value < min value
//
CorArg corArgsLongDefValueLessThanMinLimit[] =
{
  { "--LongDefValueLessThanMinLimit", NULL, CorArgLong, &longVal, CorArgOpt, _vp 9, _vp 10, _vp 20, "LongDefValueLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsULongDefValueLessThanMinLimit (40) - ulong def value < min value
//
CorArg corArgsULongDefValueLessThanMinLimit[] =
{
  { "--ULongDefValueLessThanMinLimit", NULL, CorArgULong, &ulongVal, CorArgOpt, _vp 9, _vp 10, _vp 20, "ULongDefValueLessThanMinLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsCharDefValueExceedsMaxLimit (41) - char def value > max value                                
//
CorArg corArgsCharDefValueExceedsMaxLimit[] =
{
  { "--CharDefValueExceedsMaxLimit", NULL, CorArgChar, &charVal, CorArgOpt, _vp 22, _vp 10, _vp 20, "CharDefValueExceedsMaxLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsUCharDefValueExceedsMaxLimit (42) - uchar def value > max value                               
//
CorArg corArgsUCharDefValueExceedsMaxLimit[] =
{
  { "--UCharDefValueExceedsMaxLimit", NULL, CorArgUChar, &ucharVal, CorArgOpt, _vp 22, _vp 10, _vp 20, "UCharDefValueExceedsMaxLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsShortDefValueExceedsMaxLimit (43) - short def value > max value                               
//
CorArg corArgsShortDefValueExceedsMaxLimit[] =
{
  { "--ShortDefValueExceedsMaxLimit", NULL, CorArgShort, &shortVal, CorArgOpt, _vp 22, _vp 10, _vp 20, "ShortDefValueExceedsMaxLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsUShortDefValueExceedsMaxLimit (44) - ushort def value > max value                              
//
CorArg corArgsUShortDefValueExceedsMaxLimit[] =
{
  { "--UShortDefValueExceedsMaxLimit", NULL, CorArgUShort, &ushortVal, CorArgOpt, _vp 22, _vp 10, _vp 20, "UShortDefValueExceedsMaxLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsIntDefValueExceedsMaxLimit (45) - int def value > max value                                 
//
CorArg corArgsIntDefValueExceedsMaxLimit[] =
{
  { "--IntDefValueExceedsMaxLimit", NULL, CorArgInt, &intVal, CorArgOpt, _vp 22, _vp 10, _vp 20, "IntDefValueExceedsMaxLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsUIntDefValueExceedsMaxLimit (46) - uint def value > max value                                
//
CorArg corArgsUIntDefValueExceedsMaxLimit[] =
{
  { "--UIntDefValueExceedsMaxLimit", NULL, CorArgUInt, &uintVal, CorArgOpt, _vp 22, _vp 10, _vp 20, "UIntDefValueExceedsMaxLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsLongDefValueExceedsMaxLimit (47) - long def value > max value
//
CorArg corArgsLongDefValueExceedsMaxLimit[] =
{
  { "--LongDefValueExceedsMaxLimit", NULL, CorArgLong, &longVal, CorArgOpt, _vp 22, _vp 10, _vp 20, "LongDefValueExceedsMaxLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsULongDefValueExceedsMaxLimit (48) - ulong def value > max value
//
CorArg corArgsULongDefValueExceedsMaxLimit[] =
{
  { "--ULongDefValueExceedsMaxLimit", NULL, CorArgULong, &ulongVal, CorArgOpt, _vp 22, _vp 10, _vp 20, "ULongDefValueExceedsMaxLimit" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsNoOptionName (49) - no longName nor shortName
//
CorArg corArgsNoOptionName[] =
{
  { NULL, NULL, CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "no option name" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsInvalidType (50) - Invalid CorArgType                                        
//
CorArg corArgsInvalidType[] =
{
  { "--InvalidType", NULL, (CorArgType) 101, &b1, CorArgOpt, _vp false, _vp false, _vp true, "Invalid CorArgType" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsInvalidSort (51) - Invalid CorArgSort                                        
//
CorArg corArgsInvalidSort[] =
{
  { "--InvalidSort", NULL, CorArgBool, &b1, (CorArgSort) 101, _vp false, _vp false, _vp true, "Invalid CorArgSort" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsNullValue (52) - NULL valueP                                               
//
CorArg corArgsNullValue[] =
{
  { "--NullValue", NULL, CorArgBool, NULL, CorArgOpt, _vp false,    _vp false, _vp true, "NULL valueP" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsNullDescription (53) - NULL description                                          
//
CorArg corArgsNullDescription[] =
{
  { "--NullDescription", NULL, CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, NULL },
  CORARGS_END
};




// -----------------------------------------------------------------------------
//
// corArgsShortNameTakenByBuiltinShortName (54) -
//
CorArg corArgsShortNameTakenByBuiltinShortName[] =
{
  { "--XX", "-u", CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "-u" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsShortNameTakenByBuiltinLongName (55) -
//
CorArg corArgsShortNameTakenByBuiltinLongName[] =
{
  { NULL, "--usage", CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "-u" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsLongNameTakenByBuiltinShortName (56) -
//
CorArg corArgsLongNameTakenByBuiltinShortName[] =
{
  { "-u", NULL, CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "--usage" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsLongNameTakenByBuiltinLongName (57) -
//
CorArg corArgsLongNameTakenByBuiltinLongName[] =
{
  { "--usage", NULL, CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "--usage" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsShortNameTakenByOptionShortName (58) -
//
CorArg corArgsShortNameTakenByOptionShortName[] =
{
  { NULL, "-s", CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "-s as shortName" },
  { NULL, "-s", CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "-s as shortName" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsShortNameTakenByOptionLongName (59) -
//
CorArg corArgsShortNameTakenByOptionLongName[] =
{
  { "-b",    "-1", CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "--bool as longName" },
  { "-bool", "-b", CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "--bool as longName" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsLongNameTakenByOptionShortName (60) -
//
CorArg corArgsLongNameTakenByOptionShortName[] =
{
  { "--ss", "-s", CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "-s as shortName" },
  { "-s", " -s2", CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "-s as longName"    },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsLongNameTakenByOptionLongName (61) -
//
CorArg corArgsLongNameTakenByOptionLongName[] =
{
  { "--bool", NULL, CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "--bool as longName" },
  { "--bool", NULL, CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "--bool as longName" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// corArgsShortLongNameIsTheSame (62) - this one should work
//
CorArg corArgsShortLongNameIsTheSame[] =
{
  { "-b", "-b", CorArgBool, &b1, CorArgOpt, _vp false,    _vp false, _vp true, "-b as both short and long name" },
  CORARGS_END
};



// -----------------------------------------------------------------------------
//
// main - 
//
// -----------------------------------------------------------------------------
//
// testLog - this program's log: stdout, as the old KBL_* printfs were
//
// Errors, warnings and exits always; verbose lines with -v. Under --ktest the line
// number is always 0 - line numbers change and would break every expected output.
//
static bool verbose       = false;
static bool noLineNumbers = false;

static void testLog(const char* fileName, int lineNo, const char* functionName, char type, int aux, const char* format, ...)
{
  va_list ap;

  (void) aux;

  if ((type != 'E') && (type != 'W') && (type != 'X') && ((type != 'V') || (verbose == false)))
    return;

  printf("%c:%s[%d]: %s: ", type, fileName, (noLineNumbers == true)? 0 : lineNo, functionName);
  va_start(ap, format);
  vprintf(format, ap);
  va_end(ap);
  printf("\n");
  fflush(stdout);
}



// -----------------------------------------------------------------------------
//
// corArgsErrorPresent -
//
static void corArgsErrorPresent(void)
{
  CorArgsError* eP = corArgsErrorGet();
  int         eNo = 1;

  while (eP != NULL)
  {
    if (verbose)
      printf("%s[%d]:%s: %s: %s\n", eP->fileName, eP->lineNo, eP->funcName, eP->optionName, eP->description);
    else
      printf("Error %d: %s: %s\n", eNo, eP->optionName, eP->description);

    eP = eP->next;
    ++eNo;
  }
}



// -----------------------------------------------------------------------------
//
// main - 
//
int main(int argC, char* argV[])
{
  CorArgsStatus  ks;
  char*        peekValue;
  
  if (corArgsPeek(argC, argV, kargs, "--ktest") != NULL)
    noLineNumbers = true;

  if (corArgsPeek(argC, argV, kargs, "-v") != NULL)
    verbose = true;

  corBaseInit(testLog);
  COR_LIB_V("Calling corArgsPeek");

  if (corArgsPeek(argC, argV, kargs, "--peek") != NULL)
  {
    printf("--peek is SET\n");
    return 0;
  }
  
  COR_LIB_V("Calling corArgsPeek again");

  if ((peekValue = corArgsPeek(argC, argV, kargs, "--speek")) != NULL)
  {
    printf("--speek is '%s'\n", peekValue);
    return 0;
  }

  if ((peekValue = corArgsPeek(argC, argV, kargs, "--case")) != NULL)
  {
    if (strcmp(peekValue, "MinCharOutOfBounds") == 0)
    {
      char* caseName = "MinCharOutOfBounds";

      ks = corArgsInit("corArgsTest", corArgsMinCharOutOfBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "MinUCharOutOfBounds") == 0)
    {
      char* caseName = "MinUCharOutOfBounds";

      ks = corArgsInit("corArgsTest", corArgsMinUCharOutOfBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "MinShortOutOfBounds") == 0)
    {
      char* caseName = "MinShortOutOfBounds";

      ks = corArgsInit("corArgsTest", corArgsMinShortOutOfBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "MinUShortOutOfBounds") == 0)
    {
      char* caseName = "MinUShortOutOfBounds";

      ks = corArgsInit("corArgsTest", corArgsMinUShortOutOfBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "MinIntOutOfBounds") == 0)
    {
      char* caseName = "MinIntOutOfBounds";

      ks = corArgsInit("corArgsTest", corArgsMinIntOutOfBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "MinUIntOutOfBounds") == 0)
    {
      char* caseName = "MinUIntOutOfBounds";

      ks = corArgsInit("corArgsTest", corArgsMinUIntOutOfBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "MaxCharOutOfBounds") == 0)
    {
      char* caseName = "MaxCharOutOfBounds";

      ks = corArgsInit("corArgsTest", corArgsMaxCharOutOfBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "MaxUCharOutOfBounds") == 0)
    {
      char* caseName = "MaxUCharOutOfBounds";

      ks = corArgsInit("corArgsTest", corArgsMaxUCharOutOfBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "MaxShortOutOfBounds") == 0)
    {
      char* caseName = "MaxShortOutOfBounds";

      ks = corArgsInit("corArgsTest", corArgsMaxShortOutOfBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "MaxUShortOutOfBounds") == 0)
    {
      char* caseName = "MaxUShortOutOfBounds";

      ks = corArgsInit("corArgsTest", corArgsMaxUShortOutOfBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "MaxIntOutOfBounds") == 0)
    {
      char* caseName = "MaxIntOutOfBounds";

      ks = corArgsInit("corArgsTest", corArgsMaxIntOutOfBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "MaxUIntOutOfBounds") == 0)
    {
      char* caseName = "MaxUIntOutOfBounds";

      ks = corArgsInit("corArgsTest", corArgsMaxUIntOutOfBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "DefCharOutOfMaxBounds") == 0)
    {
      char* caseName = "DefCharOutOfMaxBounds";

      ks = corArgsInit("corArgsTest", corArgsDefCharOutOfMaxBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "DefCharOutOfMinBounds") == 0)
    {
      char* caseName = "DefCharOutOfMinBounds";

      ks = corArgsInit("corArgsTest", corArgsDefCharOutOfMinBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "DefUCharOutOfMaxBounds") == 0)
    {
      char* caseName = "DefUCharOutOfMaxBounds";

      ks = corArgsInit("corArgsTest", corArgsDefUCharOutOfMaxBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "DefUCharOutOfMinBounds") == 0)
    {
      char* caseName = "DefUCharOutOfMinBounds";

      ks = corArgsInit("corArgsTest", corArgsDefUCharOutOfMinBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "DefShortOutOfMaxBounds") == 0)
    {
      char* caseName = "DefShortOutOfMaxBounds";

      ks = corArgsInit("corArgsTest", corArgsDefShortOutOfMaxBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "DefShortOutOfMinBounds") == 0)
    {
      char* caseName = "DefShortOutOfMinBounds";

      ks = corArgsInit("corArgsTest", corArgsDefShortOutOfMinBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "DefUShortOutOfMaxBounds") == 0)
    {
      char* caseName = "DefUShortOutOfMaxBounds";

      ks = corArgsInit("corArgsTest", corArgsDefUShortOutOfMaxBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "DefUShortOutOfMinBounds") == 0)
    {
      char* caseName = "DefUShortOutOfMinBounds";

      ks = corArgsInit("corArgsTest", corArgsDefUShortOutOfMinBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "DefIntOutOfMaxBounds") == 0)
    {
      char* caseName = "DefIntOutOfMaxBounds";

      ks = corArgsInit("corArgsTest", corArgsDefIntOutOfMaxBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "DefIntOutOfMinBounds") == 0)
    {
      char* caseName = "DefIntOutOfMinBounds";

      ks = corArgsInit("corArgsTest", corArgsDefIntOutOfMinBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "DefUIntOutOfMaxBounds") == 0)
    {
      char* caseName = "DefUIntOutOfMaxBounds";

      ks = corArgsInit("corArgsTest", corArgsDefUIntOutOfMaxBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "DefUIntOutOfMinBounds") == 0)
    {
      char* caseName = "DefUIntOutOfMinBounds";

      ks = corArgsInit("corArgsTest", corArgsDefUIntOutOfMinBounds, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }


    else if (strcmp(peekValue, "CharMaxLimitLessThanMinLimit") == 0)
    {
      char* caseName = "CharMaxLimitLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsCharMaxLimitLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "UCharMaxLimitLessThanMinLimit") == 0)
    {
      char* caseName = "UCharMaxLimitLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsUCharMaxLimitLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "ShortMaxLimitLessThanMinLimit") == 0)
    {
      char* caseName = "ShortMaxLimitLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsShortMaxLimitLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "UShortMaxLimitLessThanMinLimit") == 0)
    {
      char* caseName = "UShortMaxLimitLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsUShortMaxLimitLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "IntMaxLimitLessThanMinLimit") == 0)
    {
      char* caseName = "IntMaxLimitLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsIntMaxLimitLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "UIntMaxLimitLessThanMinLimit") == 0)
    {
      char* caseName = "UIntMaxLimitLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsUIntMaxLimitLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "LongMaxLimitLessThanMinLimit") == 0)
    {
      char* caseName = "LongMaxLimitLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsLongMaxLimitLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "ULongMaxLimitLessThanMinLimit") == 0)
    {
      char* caseName = "ULongMaxLimitLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsULongMaxLimitLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "CharDefValueLessThanMinLimit") == 0)
    {
      char* caseName = "CharDefValueLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsCharDefValueLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "UCharDefValueLessThanMinLimit") == 0)
    {
      char* caseName = "UCharDefValueLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsUCharDefValueLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "ShortDefValueLessThanMinLimit") == 0)
    {
      char* caseName = "ShortDefValueLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsShortDefValueLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "UShortDefValueLessThanMinLimit") == 0)
    {
      char* caseName = "UShortDefValueLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsUShortDefValueLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "IntDefValueLessThanMinLimit") == 0)
    {
      char* caseName = "IntDefValueLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsIntDefValueLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "IntDefValueLessThanMinLimit") == 0)
    {
      char* caseName = "IntDefValueLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsIntDefValueLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "UIntDefValueLessThanMinLimit") == 0)
    {
      char* caseName = "UIntDefValueLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsUIntDefValueLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "LongDefValueLessThanMinLimit") == 0)
    {
      char* caseName = "LongDefValueLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsLongDefValueLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "ULongDefValueLessThanMinLimit") == 0)
    {
      char* caseName = "ULongDefValueLessThanMinLimit";

      ks = corArgsInit("corArgsTest", corArgsULongDefValueLessThanMinLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "CharDefValueExceedsMaxLimit") == 0)
    {
      char* caseName = "CharDefValueExceedsMaxLimit";

      ks = corArgsInit("corArgsTest", corArgsCharDefValueExceedsMaxLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "UCharDefValueExceedsMaxLimit") == 0)
    {
      char* caseName = "UCharDefValueExceedsMaxLimit";

      ks = corArgsInit("corArgsTest", corArgsUCharDefValueExceedsMaxLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "ShortDefValueExceedsMaxLimit") == 0)
    {
      char* caseName = "ShortDefValueExceedsMaxLimit";

      ks = corArgsInit("corArgsTest", corArgsShortDefValueExceedsMaxLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "UShortDefValueExceedsMaxLimit") == 0)
    {
      char* caseName = "UShortDefValueExceedsMaxLimit";

      ks = corArgsInit("corArgsTest", corArgsUShortDefValueExceedsMaxLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "IntDefValueExceedsMaxLimit") == 0)
    {
      char* caseName = "IntDefValueExceedsMaxLimit";

      ks = corArgsInit("corArgsTest", corArgsIntDefValueExceedsMaxLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "UIntDefValueExceedsMaxLimit") == 0)
    {
      char* caseName = "UIntDefValueExceedsMaxLimit";

      ks = corArgsInit("corArgsTest", corArgsUIntDefValueExceedsMaxLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "LongDefValueExceedsMaxLimit") == 0)
    {
      char* caseName = "LongDefValueExceedsMaxLimit";

      ks = corArgsInit("corArgsTest", corArgsLongDefValueExceedsMaxLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "ULongDefValueExceedsMaxLimit") == 0)
    {
      char* caseName = "ULongDefValueExceedsMaxLimit";

      ks = corArgsInit("corArgsTest", corArgsULongDefValueExceedsMaxLimit, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "NoOptionName") == 0)
    {
      char* caseName = "NoOptionName";

      ks = corArgsInit("corArgsTest", corArgsNoOptionName, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "InvalidType") == 0)
    {
      char* caseName = "InvalidType";

      ks = corArgsInit("corArgsTest", corArgsInvalidType, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "InvalidSort") == 0)
    {
      char* caseName = "InvalidSort";

      ks = corArgsInit("corArgsTest", corArgsInvalidSort, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "NullValue") == 0)
    {
      char* caseName = "NullValue";

      ks = corArgsInit("corArgsTest", corArgsNullValue, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "NullDescription") == 0)
    {
      char* caseName = "NullDescription";

      ks = corArgsInit("corArgsTest", corArgsNullDescription, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "ShortNameTakenByBuiltinShortName") == 0)
    {
      char* caseName = "ShortNameTakenByBuiltinShortName";

      ks = corArgsInit("corArgsTest", corArgsShortNameTakenByBuiltinShortName, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "ShortNameTakenByBuiltinLongName") == 0)
    {
      char* caseName = "ShortNameTakenByBuiltinLongName";

      ks = corArgsInit("corArgsTest", corArgsShortNameTakenByBuiltinLongName, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "LongNameTakenByBuiltinShortName") == 0)
    {
      char* caseName = "LongNameTakenByBuiltinShortName";

      ks = corArgsInit("corArgsTest", corArgsLongNameTakenByBuiltinShortName, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "LongNameTakenByBuiltinLongName") == 0)
    {
      char* caseName = "LongNameTakenByBuiltinLongName";

      ks = corArgsInit("corArgsTest", corArgsLongNameTakenByBuiltinLongName, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "ShortNameTakenByOptionShortName") == 0)
    {
      char* caseName = "ShortNameTakenByOptionShortName";

      ks = corArgsInit("corArgsTest", corArgsShortNameTakenByOptionShortName, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }
      
      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "ShortNameTakenByOptionLongName") == 0)
    {
      char* caseName = "ShortNameTakenByOptionLongName";

      ks = corArgsInit("corArgsTest", corArgsShortNameTakenByOptionLongName, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }

      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "LongNameTakenByOptionShortName") == 0)
    {
      char* caseName = "LongNameTakenByOptionShortName";

      ks = corArgsInit("corArgsTest", corArgsLongNameTakenByOptionShortName, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }

      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "LongNameTakenByOptionLongName") == 0)
    {
      char* caseName = "LongNameTakenByOptionLongName";

      ks = corArgsInit("corArgsTest", corArgsLongNameTakenByOptionLongName, "CORARGSTEST");
      if (ks == CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit did not detect %s", caseName);
      }

      printf("OK: %s error correctly detected\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
    else if (strcmp(peekValue, "ShortLongNameIsTheSame") == 0)
    {
      char* caseName = "ShortLongNameIsTheSame";

      ks = corArgsInit("corArgsTest", corArgsShortLongNameIsTheSame, "CORARGSTEST");
      if (ks != CorArgsOk)
      {
        corArgsErrorPresent();
        COR_LIB_X(1, "corArgsInit incorrectly flagged error for %s", caseName);
      }
      
      printf("OK: no error for %s\n", caseName);
      corArgsErrorPresent();
      exit(0);
    }
  }

  COR_LIB_V("Calling corArgsInit");
  ks = corArgsInit("corArgsTest", kargs, "CORARGSTEST");
  if (ks != CorArgsOk)
  {
    corArgsErrorPresent();
    COR_LIB_X(1, "error initializing corArgs library (error %d)", ks);
  }

  COR_LIB_V("Calling corArgsConfig");
  corArgsConfig(CorArgsPrefix, "KAT_");

  COR_LIB_V("Calling corArgsParse");
  ks = corArgsParse(argC, argV);
  COR_LIB_V("Back from corArgsParse");

  if (ks != CorArgsOk)
  {
    corArgsErrorPresent();
    exit(1);
  }

  // corArgsPeek("-u");


  printf("cMin:      %d\n", cMin);
  printf("cMax:      %d\n", cMax);
  printf("cMinMax:   %d\n", cMinMax);
  printf("charVal:   %d\n", charVal);
  
  printf("sMin:      %d\n", sMin);
  printf("sMax:      %d\n", sMax);
  printf("sMinMax:   %d\n", sMinMax);
  printf("shortVal:  %d\n", shortVal);

  printf("iMin:      %d\n", iMin);
  printf("iMax:      %d\n", iMax);
  printf("iMinMax:   %d\n", iMinMax);
  printf("intVal:    %d\n", intVal);

  printf("llMin:     %lld\n", llMin);
  printf("llMax:     %lld\n", llMax);
  printf("llMinMax:  %lld\n", llMinMax);
  printf("longVal:   %lld\n", longVal);

  printf("ucMin:     %d\n", ucMin);
  printf("ucMax:     %d\n", ucMax);
  printf("ucMinMax:  %d\n", ucMinMax);

  printf("usMin:     %d\n", usMin);
  printf("usMax:     %d\n", usMax);
  printf("usMinMax:  %d\n", usMinMax);

  printf("uiMin:     %d\n", uiMin);
  printf("uiMax:     %d\n", uiMax);
  printf("uiMinMax:  %d\n", uiMinMax);

  printf("ullMin:    %llu\n", ullMin);
  printf("ullMax:    %llu\n", ullMax);
  printf("ullMinMax: %llu\n", ullMinMax);

  printf("fMin:      %f\n", fMin);
  printf("fMax:      %f\n", fMax);
  printf("fMinMax:   %f\n", fMinMax);

  printf("strMin:    %s\n", strMin);
  printf("strMax:    %s\n", strMax);
  printf("strMinMax: %s\n", strMinMax);

  printf("b1:        %s\n", COR_FT(b1));
  printf("b2:        %s\n", COR_FT(b2));
  printf("b3:        %s\n", COR_FT(b3));

  return 0;
}
