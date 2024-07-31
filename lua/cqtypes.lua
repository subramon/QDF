-- TODO P2 Figure out how to keep these in sync with enum in C
local cqtypes = {}
cqtypes["I1"]   =  "int8_t"
cqtypes["UI1"]  =  "uint8_t"

cqtypes["I2"]   =  "int16_t"
cqtypes["UI2"]  =  "uint16_t"

cqtypes["I4"]   =  "int32_t"
cqtypes["UI4"]  =  "uint32_t"

cqtypes["I8"]   =  "int64_t"
cqtypes["UI8"]  =  "uint64_t"

cqtypes["F4"]  =  "float"
cqtypes["F8"]  =  "double"

return cqtypes
