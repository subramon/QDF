-- Template is used for add, sub, mul, div for ispc 
-- z := x + y 
return require 'Q/UTILS/lua/code_gen' { 
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
export void
${fn}(
    uniform ${ispctype} x[],
    uniform ${ispctype} y[],
    uniform uint32 n,
    uniform ${ispctype} z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i] ${operator} y[i];
  }

}
]],
}
