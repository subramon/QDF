local lQDF = require 'lQDF'
local tests = {}
tests.t1 = function()
  local x = lQDF.mk_df()
  assert(type(x) == "lQDF")
  x:pr_df_as_csv()
  print("Success on Test t1")
end
tests.t1()


-- return tests
