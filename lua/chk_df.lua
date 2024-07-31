local function chk_df(df)
  assert(type(df) == "lQDF")
  assert(df:check()) 
  assert(df:is_df() == true)
  local inn  = df:obj_arr_len(); assert(inn >= 0)
  local insz = df:obj_arr_size() assert(inn <= insz)
  return true 
end
return chk_df
