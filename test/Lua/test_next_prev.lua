local lRBC = require 'lRBC'
local tests = {}

tests.t1 = function()
  for _, qtype in ipairs({"I1", "I2", "I4", "I8",}) do 
    local x = lRBC({1, 2, 3, 4, 5, 6,7, 8, 9, })
    x = x:convert(qtype)
    assert(x:is_next(">") == true)
    assert(x:is_next(">=") == true)
    assert(x:is_next("<") == false)
    assert(x:is_next("<=") == false)
    assert(x:is_next("==") == false)
    assert(x:is_next("!=") == true)

    local x = lRBC({1, 1, 1, 1, 1, 1, 1, 1, 1,})
    x = x:convert(qtype)
    assert(x:is_next(">") == false)
    assert(x:is_next(">=") == true)
    assert(x:is_next("<") == false)
    assert(x:is_next("<=") == true)
    assert(x:is_next("==") == true)
    assert(x:is_next("!=") == false)

    local x = lRBC({9, 8, 7, 6, 5, 4, 3, 2, 1, })
    x = x:convert(qtype)
    assert(x:is_next(">") == false)
    assert(x:is_next(">=") == false)
    assert(x:is_next("<") == true)
    assert(x:is_next("<=") == true)
    assert(x:is_next("==") == false)
    assert(x:is_next("!=") == true)

  end
  print("Test t1 completed successfully")
end
tests.t1()
