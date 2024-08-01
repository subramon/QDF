-- Template is used for const
return require 'RSUTILS/lua/code_gen' { 
  declaration = [[
#include "incs.h"
#include "qtypes.h"
extern void
${fn}(
    ${ctype} *out,
    uint32_t n,
    ${ctype} val
    );
    ]],
    definition =  [[
#include "${fn}.h" 
void
${fn}(
    ${ctype} *out,
    uint32_t n,
    ${ctype} val
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    out[i] = val;
  }
}
    ]],
  }
