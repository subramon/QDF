export void
vvdiv_F4(
    uniform float x[],
    uniform float y[],
    uniform uint32 n,
    uniform float z[]
    )
{
  foreach ( i = 0 ... n ) {
    z[i] = x[i] / y[i];
  }

}
