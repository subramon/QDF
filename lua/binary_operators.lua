local T = {}
-- keys are binary operators
-- value is true/true. true => possible to perform in situ operations
T.add = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.sub = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.mul = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.div = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.rem = { "I1", "I2", "I4", "I8", }
T.max = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.min = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.bitwise_and = { "I1", "I2", "I4", "I8", }
T.bitwise_or  = { "I1", "I2", "I4", "I8", }
T.bitwise_xor = { "I1", "I2", "I4", "I8", }
T.shift_left = { "I1", "I2", "I4", "I8", }
T.shift_right  = { "I1", "I2", "I4", "I8", }
T.raised_to = { "F4", "F8", }
--========================================
T.eq  = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.neq = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.gt  = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.geq = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.lt  = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.leq = { "I1", "I2", "I4", "I8", "F4", "F8", } 
T.logical_and = { "I1", "I2", "I4", "I8", }
T.logical_or  = { "I1", "I2", "I4", "I8", }


return T
