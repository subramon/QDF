local T = {}
-- keys are unary operators
-- values are valid types
T.bitwise_not = { "I1", "I2", "I4", "I8", }
T.convert = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.exp = { "F4", "F8", } 
T.log = { "F4", "F8", } 
T.abs = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.logical_not = { "I1", "I2", "I4", "I8", }
T.reciprocal = { "F4", "F8", } 
T.round = { "F4", "F8", } 
T.sawtooth = {  "I1", "I2", "I4", "I8", }
T.flatten_sawtooth = {  "I1", "I2", "I4", "I8", }
return T
