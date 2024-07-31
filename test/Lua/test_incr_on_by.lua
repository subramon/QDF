local lRBC = require 'lRBC'
local simdjson = require 'simdjson'

local tests = {}
tests.t1 = function()
  
  for _, qtype in ipairs({ "I1", "I2", "I4", "I8", "F4", "F8", }) do 
    local x = lRBC({0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0 }):convert(qtype)
    assert(x:is_sawtooth())
    local z = x:incr_on_by(0, 1, false)
    assert(z:qtype() == x:qtype())
    assert(z:is_sawtooth())
    local y = lRBC({1, 2, 3, 3, 3, 3, 4, 5, 5, 5, 5, 5, 6 }):convert(qtype)
    assert(z:qtype() == y:qtype())
    assert(y:neq(z):sum() == 0)
    assert(y:is_sawtooth())

    if ( ( qtype == "F4" ) or ( qtype == "F8" ) )  then
      -- nothing to do 
    else
      local w = z:flatten_sawtooth()
      local v = lRBC({6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }):convert(qtype)
      assert(v:neq(w):sum() == 0)
    end
    -- redo with reset == true 
    local z = x:incr_on_by(0, 1, true)
    assert(z:qtype() == x:qtype())
    assert(z:is_sawtooth())
    local y = lRBC({1, 2, 3, 0, 0, 0, 1, 2, 0, 0, 0, 0, 1 }):convert(qtype)
    assert(z:qtype() == y:qtype())
    assert(y:neq(z):sum() == 0)
    assert(y:is_sawtooth())
    if ( ( qtype == "F4" ) or ( qtype == "F8" ) )  then
      -- nothing to do 
    else
      local w = z:flatten_sawtooth()
      local v = lRBC({3, 3, 3, 0, 0, 0, 2, 2, 0, 0, 0, 0, 1 }):convert(qtype)
      assert(v:neq(w):sum() == 0)
    end
  end
  for _, qtype in ipairs({ "I1", "I2", "I4", "I8", "F4", "F8", }) do 
    local x = lRBC({0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0 }):convert(qtype)
    assert(x:is_sawtooth())
    local z = x:incr_on_by(1, 2, false)
    assert(z:is_sawtooth())
    assert(z:qtype() == x:qtype())
    local y = lRBC({0, 2, 2, 2, 4, 6, 6, 6, 6, 8, 10, 12, 12 }):convert(qtype)
    assert(y:is_sawtooth())
    assert(z:qtype() == y:qtype())
    assert(y:neq(z):sum() == 0)
    -- redo with reset = true 
    local z = x:incr_on_by(1, 2, true)
    assert(z:is_sawtooth())
    assert(z:qtype() == x:qtype())
    local y = lRBC({0, 2, 0, 0, 2, 4, 0, 0, 0, 2, 4, 6, 0 }):convert(qtype)
    assert(z:qtype() == y:qtype())
    assert(y:neq(z):sum() == 0)
    assert(y:is_sawtooth())
  end
  for _, qtype in ipairs({ "I1", "I2", "I4", "I8", "F4", "F8", }) do 
    local y = lRBC({3, 2, 1, 0, 2, 4, 0, 0, 0, 2, 4, 6, 0 }):convert(qtype)
    assert(y:is_sawtooth() == false)
  end
  print("Test t1 completed successfully")
end
tests.t1()
