local function qtype_to_ctype(qtype)
  if ( qtype == "I1" ) then return "int8_t" end 
  if ( qtype == "I2" ) then return "int16_t" end 
  if ( qtype == "I4" ) then return "int32_t" end 
  if ( qtype == "I8" ) then return "int64_t" end 

  if ( qtype == "UI1" ) then return "uint8_t" end 
  if ( qtype == "UI2" ) then return "uint16_t" end 
  if ( qtype == "UI4" ) then return "uint32_t" end 
  if ( qtype == "UI8" ) then return "uint64_t" end 

  if ( qtype == "F4" ) then return "float" end 
  if ( qtype == "F8" ) then return "double" end 
  if ( qtype == "TM" ) then return "tm_t" end 
  print("qtype = ", qtype)
  return nil
end
return qtype_to_ctype
