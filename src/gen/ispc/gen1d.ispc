export void
ispc_vvadd_I1(
    uniform int8 x[],
    uniform int8 y[],
    uniform uint32 n,
    uniform int8 z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  +  y[i];
  }

}
export void
ispc_vvadd_I2(
    uniform int16 x[],
    uniform int16 y[],
    uniform uint32 n,
    uniform int16 z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  +  y[i];
  }

}
export void
ispc_vvadd_I4(
    uniform int32 x[],
    uniform int32 y[],
    uniform uint32 n,
    uniform int32 z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  +  y[i];
  }

}
export void
ispc_vvadd_I8(
    uniform int64 x[],
    uniform int64 y[],
    uniform uint32 n,
    uniform int64 z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  +  y[i];
  }

}
export void
ispc_vvadd_F4(
    uniform float x[],
    uniform float y[],
    uniform uint32 n,
    uniform float z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  +  y[i];
  }

}
export void
ispc_vvadd_F8(
    uniform double x[],
    uniform double y[],
    uniform uint32 n,
    uniform double z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  +  y[i];
  }

}
export void
ispc_vvsub_I1(
    uniform int8 x[],
    uniform int8 y[],
    uniform uint32 n,
    uniform int8 z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  -  y[i];
  }

}
export void
ispc_vvsub_I2(
    uniform int16 x[],
    uniform int16 y[],
    uniform uint32 n,
    uniform int16 z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  -  y[i];
  }

}
export void
ispc_vvsub_I4(
    uniform int32 x[],
    uniform int32 y[],
    uniform uint32 n,
    uniform int32 z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  -  y[i];
  }

}
export void
ispc_vvsub_I8(
    uniform int64 x[],
    uniform int64 y[],
    uniform uint32 n,
    uniform int64 z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  -  y[i];
  }

}
export void
ispc_vvsub_F4(
    uniform float x[],
    uniform float y[],
    uniform uint32 n,
    uniform float z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  -  y[i];
  }

}
export void
ispc_vvsub_F8(
    uniform double x[],
    uniform double y[],
    uniform uint32 n,
    uniform double z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  -  y[i];
  }

}
export void
ispc_vvmul_I1(
    uniform int8 x[],
    uniform int8 y[],
    uniform uint32 n,
    uniform int8 z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  *  y[i];
  }

}
export void
ispc_vvmul_I2(
    uniform int16 x[],
    uniform int16 y[],
    uniform uint32 n,
    uniform int16 z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  *  y[i];
  }

}
export void
ispc_vvmul_I4(
    uniform int32 x[],
    uniform int32 y[],
    uniform uint32 n,
    uniform int32 z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  *  y[i];
  }

}
export void
ispc_vvmul_I8(
    uniform int64 x[],
    uniform int64 y[],
    uniform uint32 n,
    uniform int64 z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  *  y[i];
  }

}
export void
ispc_vvmul_F4(
    uniform float x[],
    uniform float y[],
    uniform uint32 n,
    uniform float z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  *  y[i];
  }

}
export void
ispc_vvmul_F8(
    uniform double x[],
    uniform double y[],
    uniform uint32 n,
    uniform double z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  *  y[i];
  }

}
export void
ispc_vvdiv_F4(
    uniform float x[],
    uniform float y[],
    uniform uint32 n,
    uniform float z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  /  y[i];
  }

}
export void
ispc_vvdiv_F8(
    uniform double x[],
    uniform double y[],
    uniform uint32 n,
    uniform double z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i]  /  y[i];
  }

}
