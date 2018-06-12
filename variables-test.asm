      TITLE variables-test.c
      .686P
      .model flat

INCLUDELIB MSVCRTD 
INCLUDELIB OLDNAMES

_DATA    SEGMENT
COMM  _a:DWORD
COMM  _b:DWORD
COMM  _c:DWORD
_DATA    ENDS

_DATA    SEGMENT
COMM  _this:DWORD
COMM  _is:DWORD
COMM  _more:DWORD
COMM  _variables:DWORD
_DATA    ENDS

_TEXT    SEGMENT
_param1$ = 8
_param2$ = 12
_param3$ = 16
_local1$1 = -4
_local2$2 = -8
_local3$3 = -12
_qwerty$4 = -16
_qq$5 = -20
_pp$6 = -24
_rrr$7 = -28
_ttt$8 = -32
_TEXT    ENDS

_DATA    SEGMENT
COMM  _www:DWORD
COMM  _xxx:DWORD
_DATA    ENDS

_TEXT    SEGMENT
_vvv$ = 8
_zzz$ = 12
_ccc$1 = -4
_nnn$2 = -8
_mmm$3 = -12
_TEXT    ENDS

_DATA    SEGMENT
COMM  _last_variable:DWORD
_DATA    ENDS
END
