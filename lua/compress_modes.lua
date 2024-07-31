-- TODO P2 Figure out how to keep these in sync with enum in C
local ctypes = {}
local idx = 0
ctypes["undef"]          = idx; idx = idx + 1 
ctypes["none"]           = idx; idx = idx + 1 
ctypes["distinct_vals"]  = idx; idx = idx + 1 
ctypes["F4_to_F2"]       = idx; idx = idx + 1 
ctypes["week_to_offset"] = idx; idx = idx + 1 

return ctypes
