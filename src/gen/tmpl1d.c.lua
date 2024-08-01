-- Template is used for add, sub, mul, div for ispc 
-- z := x + y 
return require 'RSUTILS/lua/code_gen' { 
  declaration = [[
extern  void
${fn}(
    ${ctype} *x,
    ${ctype} *y,
    uint32_t n,
    ${ctype} *z
    );
  ]],
  definition = [[
#include <stdint.h>
#include "${fn}.h"
void
${fn}(
    ${ctype} *x,
    ${ctype} *y,
    uint32_t n,
    ${ctype} *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (${ctype})(x[i] ${operator} y[i]);
  }

}
]],
}
