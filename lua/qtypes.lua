-- TODO P2 Figure out how to keep these in sync with enum in C
local qtypes = {}
local idx = 0
qtypes["Q0"]  =  idx; idx = idx + 1 
qtypes["B1"]  =  idx; idx = idx + 1 
qtypes["BL"]  =  idx; idx = idx + 1 

qtypes["I1"]  =  idx; idx = idx + 1 
qtypes["I2"]  =  idx; idx = idx + 1 
qtypes["I4"]  =  idx; idx = idx + 1 
qtypes["I8"]  =  idx; idx = idx + 1 
qtypes["I16"]  =  idx; idx = idx + 1 

qtypes["F2"]  =  idx; idx = idx + 1 
qtypes["F4"]  =  idx; idx = idx + 1 
qtypes["F8"]  =  idx; idx = idx + 1 

qtypes["UI1"] =  idx; idx = idx + 1 
qtypes["UI2"] =  idx; idx = idx + 1 
qtypes["UI4"] =  idx; idx = idx + 1 
qtypes["UI8"] =  idx; idx = idx + 1 
qtypes["UI16"] =  idx; idx = idx + 1 

qtypes["SC"]  =  idx; idx = idx + 1 
qtypes["SV"]  =  idx; idx = idx + 1 
qtypes["TM"]  =  idx; idx = idx + 1 
qtypes["TM1"]  =  idx; idx = idx + 1 
qtypes["HL"]  =  idx; idx = idx + 1 

return qtypes
