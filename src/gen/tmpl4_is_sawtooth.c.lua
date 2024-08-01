-- Template is used for seq
return require 'RSUTILS/lua/code_gen' { 
  declaration = [[
#include "incs.h"
#include "qtypes.h"
extern bool
${fn}(
    const ${ctype} *const invals,
    uint32_t n
    );
    ]],
    definition =  [[
#include "${fn}.h" 
bool
${fn}(
    const ${ctype} *const invals,
    uint32_t n
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( invals[i] == 0 ) { continue; }
    if ( invals[i] < 0 ) { return false; }
    // invals[i] > 0 
    if ( i > 0 ) { 
      if ( invals[i] < invals[i-1] ) { return false; }
    }
  }
  return true;
}
    ]],
  }
