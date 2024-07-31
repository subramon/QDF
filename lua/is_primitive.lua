local function is_primitive(J)
  if ( type(J) == "nil" ) then return true end 
  if ( type(J) == "string") then  return true end 
  if ( type(J) == "number") then  return true end 
  if ( type(J) == "boolean") then  return true end 
  return false
end
return is_primitive
