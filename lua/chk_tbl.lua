local function chk_tbl(T)
  assert(type(T) == "table")
  --== Check if table or object
  local is_tbl
  local is_uniform -- relevant only if is_tbl == true 
  local tbl_type   -- relevant only if is_uniform == true 
  local is_primitive  -- relevant only if is_tbl == true 

  local nT = #T
  local idx = 1 
  local cnt = 0
  for k, v in pairs(T) do 
    cnt = cnt + 1 
  end
  assert(cnt > 0) -- TODO Need to undo this assumption
  if ( cnt ~= nT ) then 
    -- this is an object
    is_tbl = false 
  else
    is_tbl = true
    for i = 1, cnt do 
      if ( ( type(T[i]) ~= "boolean" ) and ( not T[i]) ) then
        is_tbl = false
        break
      end
    end
  end
  if ( is_tbl ) then 
    --=======================
    is_uniform = true
    tbl_type = type(T[1])
    for i = 2, nT do
      if ( tbl_type ~= type(T[i]) ) then
        is_uniform = false
        break
      end
    end
    if ( not is_uniform ) then tbl_type = nil end 
    --=======================
    is_primitive = true
    for i = 1, nT do 
      local ttype = type(T[1])
      if ( ( ttype == "boolean" ) or 
           ( ttype == "number" ) or 
           ( ttype == "string" ) ) then
      else
        is_primitive = false
        break
      end
    end
    --=======================
  end
  return is_tbl, is_uniform, is_primitive, tbl_type 
end
return chk_tbl
