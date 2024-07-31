export void
ispc_vsadd_I1(
    uniform int8 x[],
    uniform int8 sclr,
    uniform uint32 n,
    uniform int8 y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] + sclr; 
  }
}
    
export void
ispc_vsadd_I2(
    uniform int16 x[],
    uniform int16 sclr,
    uniform uint32 n,
    uniform int16 y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] + sclr; 
  }
}
    
export void
ispc_vsadd_I4(
    uniform int32 x[],
    uniform int32 sclr,
    uniform uint32 n,
    uniform int32 y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] + sclr; 
  }
}
    
export void
ispc_vsadd_I8(
    uniform int64 x[],
    uniform int64 sclr,
    uniform uint32 n,
    uniform int64 y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] + sclr; 
  }
}
    
export void
ispc_vsadd_F4(
    uniform float x[],
    uniform float sclr,
    uniform uint32 n,
    uniform float y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] + sclr; 
  }
}
    
export void
ispc_vsadd_F8(
    uniform double x[],
    uniform double sclr,
    uniform uint32 n,
    uniform double y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] + sclr; 
  }
}
    
export void
ispc_vssub_I1(
    uniform int8 x[],
    uniform int8 sclr,
    uniform uint32 n,
    uniform int8 y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] - sclr; 
  }
}
    
export void
ispc_vssub_I2(
    uniform int16 x[],
    uniform int16 sclr,
    uniform uint32 n,
    uniform int16 y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] - sclr; 
  }
}
    
export void
ispc_vssub_I4(
    uniform int32 x[],
    uniform int32 sclr,
    uniform uint32 n,
    uniform int32 y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] - sclr; 
  }
}
    
export void
ispc_vssub_I8(
    uniform int64 x[],
    uniform int64 sclr,
    uniform uint32 n,
    uniform int64 y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] - sclr; 
  }
}
    
export void
ispc_vssub_F4(
    uniform float x[],
    uniform float sclr,
    uniform uint32 n,
    uniform float y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] - sclr; 
  }
}
    
export void
ispc_vssub_F8(
    uniform double x[],
    uniform double sclr,
    uniform uint32 n,
    uniform double y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] - sclr; 
  }
}
    
export void
ispc_vsmul_I1(
    uniform int8 x[],
    uniform int8 sclr,
    uniform uint32 n,
    uniform int8 y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] * sclr; 
  }
}
    
export void
ispc_vsmul_I2(
    uniform int16 x[],
    uniform int16 sclr,
    uniform uint32 n,
    uniform int16 y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] * sclr; 
  }
}
    
export void
ispc_vsmul_I4(
    uniform int32 x[],
    uniform int32 sclr,
    uniform uint32 n,
    uniform int32 y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] * sclr; 
  }
}
    
export void
ispc_vsmul_I8(
    uniform int64 x[],
    uniform int64 sclr,
    uniform uint32 n,
    uniform int64 y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] * sclr; 
  }
}
    
export void
ispc_vsmul_F4(
    uniform float x[],
    uniform float sclr,
    uniform uint32 n,
    uniform float y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] * sclr; 
  }
}
    
export void
ispc_vsmul_F8(
    uniform double x[],
    uniform double sclr,
    uniform uint32 n,
    uniform double y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] * sclr; 
  }
}
    
export void
ispc_vsdiv_F4(
    uniform float x[],
    uniform float sclr,
    uniform uint32 n,
    uniform float y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] / sclr; 
  }
}
    
export void
ispc_vsdiv_F8(
    uniform double x[],
    uniform double sclr,
    uniform uint32 n,
    uniform double y[]
    )
{
  foreach ( i = 0 ... n ) { 
    y[i] = x[i] / sclr; 
  }
}
    
