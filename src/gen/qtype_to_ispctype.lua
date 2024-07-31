local function qtypeo_ctype(qtype)
  if ( qtype == "I1" ) then return "int8" end 
  if ( qtype == "I2" ) then return "int16" end 
  if ( qtype == "I4" ) then return "int32" end 
  if ( qtype == "I8" ) then return "int64" end 

  if ( qtype == "UI1" ) then return "uint8" end 
  if ( qtype == "UI2" ) then return "uint16" end 
  if ( qtype == "UI4" ) then return "uint32" end 
  if ( qtype == "UI8" ) then return "uint64" end 

  if ( qtype == "F4" ) then return "float" end 
  if ( qtype == "F8" ) then return "double" end 
  return nil
end
return qtypeo_ctype
