local function expt_lua_global(n)
  print("Entering function")
  print("lC = ", type(G.C))
  print(type(x))
  print(type(n))
  print("x = ", x)
  if ( n ) then print("n = ", n) end 
  print("Leaving  function")
  return true
end
return expt_lua_global
