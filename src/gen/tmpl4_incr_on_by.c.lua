-- Template is used for seq
return require 'RSUTILS/lua/code_gen' { 
  declaration = [[
#include "incs.h"
#include "qtypes.h"
extern void
${fn}(
    const ${ctype} *const invals,
    ${ctype} on,
    ${ctype} incr,
    bool reset,
    uint32_t n,
    ${ctype} *restrict outvals
    );
    ]],
    definition =  [[
#include "${fn}.h" 
void
${fn}(
    const ${ctype} *const invals,
    ${ctype} on,
    ${ctype} incr,
    bool reset,
    uint32_t n,
    ${ctype} *restrict outvals
    )
{
  outvals[0] = 0; if ( invals[0] == on ) { outvals[0] = incr; } 
  for ( uint32_t i = 1; i < n; i++ ) { 
    if ( invals[i] == on ) {
      outvals[i] = (${ctype})(outvals[i-1] + incr);
    }
    else { 
      if ( reset ) { 
        outvals[i] = 0; 
      }
      else {
        outvals[i] = (${ctype})(outvals[i-1]);
      }
    }
  }
}
    ]],
  }
