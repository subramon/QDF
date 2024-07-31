local lRBC = require 'lRBC'

local tests = {}
tests.t1 = function()
  for _, qtype in ipairs({ "F4", "F8", }) do 
    local x = lRBC({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }):convert(qtype)
    local chk_y = lRBC({0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121 }):convert(qtype)
    local y = x:raised_to(2)
    -- print(y)
    local epsilon = 0.01
    assert(y:sub(chk_y):abs():gt(epsilon):sum() == 0)
    print("Test t1 succeeded for ", qtype)

  end
end
tests.t1()
