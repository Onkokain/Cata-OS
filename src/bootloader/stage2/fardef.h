// was prev used to define fat and _cdecl so that clang doesnt throw errors; not required anymore
#pragma once
#ifdef __clang__
#define far
#define _cdecl
#endif
