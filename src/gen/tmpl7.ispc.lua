-- Template is used for const
return require 'Q/UTILS/lua/code_gen' { 
  declaration = [[
extern void
${fn}(
    ${ctype} *x,
    ${ctype} sclr,
    uint32_t n,
    ${ctype} *y
    );
    ]],
    definition =  [[
export void
${fn}(
    uniform ${ispctype} x[],
    uniform ${ispctype} sclr,
    uniform uint32 n,
    uniform ${ispctype} y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] ${operator} sclr; 
  }
}
    ]],
  }
