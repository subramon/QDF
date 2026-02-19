-- convention that in_args passed before out_args

local function mk_caller(S)
  local args_for_C = {}
  local code = {}
  local X = {} 
  X[#X+1] =  string.format("return function(")
  for k, v in ipairs(S.in_args) do
    if ( k > 1 ) then X[#X+1] =  ", " end 
    X[#X+1] =  v.name
  end
  X[#X+1] =  ")\n"
  code[#code+1] = table.concat(X)
  code[#code+1] =  "  print('HELLO WORLD RUN FUNC')\n"
  --===================================
  code[#code+1] = "  local lQDF        = require 'lQDF'\n"
  code[#code+1] = "  local lqdfmem     = require 'lqdfmem'\n"
  code[#code+1] = "  local condl_specl = require 'condl_specl'\n"
  code[#code+1] = "--==========================================\n"
  --===================================
  for k, v in ipairs(S.in_args) do 
    code[#code+1] = string.format("  assert(type(%s) == '%s')\n", 
      v.name, v.qtype)
    if ( v.qtype == "lQDF" ) then 
      code[#code+1] = string.format("  local C_%s = %s:cmem_ptr()\n", 
        v.name, v.name)
        args_for_C[#args_for_C+1] = string.format("C_%s", v.name)
    else
      assert("not implemented")
    end 
  end
  code[#code+1] = "--==========================================\n"
  --===================================
  --[[
  local get_subs = lQDF.get_get_subs("coalesce")
  local subs = get_subs(x, y)
  local cfunc, exec = condl_specl(subs)
  --]]
  code[#code+1] = 
    string.format("  local get_subs = lQDF.get_get_subs('%s')\n", S.operator)
  local X = {} 
  X[#X+1] =  string.format("  local subs = get_subs(")
  for k, v in ipairs(S.in_args) do
    if ( k >  1 ) then X[#X+1] =  ", " end 
    X[#X+1] =  v.name
  end
  X[#X+1] =  ")\n"
  code[#code+1] = table.concat(X)
  code[#code+1] = "  local cfunc, exec = condl_specl(subs)\n"
  code[#code+1] = "--==========================================\n"
  --===================================
  for k, v in ipairs(S.out_args) do
    --[[
  local z = lqdfmem(0) -- create an empty QDF for z 
  local qdf_z = setmetatable({}, lQDF)
  qdf_z._cmem = z
  local C_z = qdf_z:cmem_ptr()  -- provide C access to z
    --]]
    if ( v.qtype == "lQDF" ) then 
      code[#code+1] = 
        string.format("  local %s = lqdfmem(0)\n", v.name)
      code[#code+1] = 
        string.format("  local qdf_%s = setmetatable({}, lQDF)\n", v.name)
      code[#code+1] = 
        string.format("  qdf_%s._cmem = %s \n", v.name, v.name)
      code[#code+1] = 
        string.format("  local C_%s = qdf_%s:cmem_ptr() \n", v.name, v.name)
      args_for_C[#args_for_C+1] = string.format("C_%s", v.name)
    else
      assert("not implemented")
    end 
  end
  code[#code+1] = "--==========================================\n"
  code[#code+1] = "  local exec = assert(lQDF.q_get(cfunc))\n"
  local X = {}
  X[#X+1] = "  local status = exec[cfunc]("
  for k, v in ipairs(args_for_C) do 
    if ( k > 1 ) then X[#X+1] = ", " end 
    X[#X+1] = v 
  end
  X[#X+1] = ")\n"
  code[#code+1] = table.concat(X) 
  code[#code+1] = "  -- assert(status == 0)\n" -- TODO P0
  local X = {}
  X[#X+1] = "  return " 
  for k, v in ipairs(S.out_args) do
    if ( k > 1 ) then X[#X+1] = ", " end
    if ( v.qtype == "lQDF" ) then 
      X[#X+1] = string.format("qdf_%s",  v.name)
    end
  end
  X[#X+1] = "\n" 
  code[#code+1] = table.concat(X) 
  code[#code+1] = "end\n"

  return table.concat(code)
end

local ut = false
if ( ut ) then 
local spec = { 
  operator = "coalesce", 
  in_args =  {
    { name = "x", qtype = "lQDF", rw = "read_only", },
    { name = "y", qtype = "lQDF", rw = "read_only", },
  },
  out_args  = {
    { name = "z", qtype = "lQDF", }
  },
}
local code = mk_caller(spec)
print(code)
else
return mk_caller
end


