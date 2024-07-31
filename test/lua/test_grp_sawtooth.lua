G = {} -- to bypass strict 
G.debug= true
local plfile = require 'pl.file'
require 'strict'
local lRBC = require 'lRBC'
local tests = {}

--===============================
local cols =  { "inval", "grpval", "outval", "flatval", }
local qtypes = { "I1", "I1", "I1", "I1", }
local optargs = { is_hdr = true }
--===============================
tests.grp_sawtooth = function (infile)
  local indf = assert(lRBC.read_csv(cols, qtypes, infile, optargs))
  local g = indf:get("grpval")
  local g2 = g:sort("ascending")
  local ng = g2:srt_grp_cnt()
  local i = indf:get("inval")

  local chk_o = indf:get("outval")
  local o = i:grp_sawtooth(ng)
  assert(chk_o:neq(o):sum() == 0)

  local chk_f = indf:get("flatval")
  local f = o:grp_flatten_sawtooth(ng)
  assert(chk_f:neq(f):sum() == 0)

  print("Test grp_sawtooth completed successfully")
end
local status = tests.grp_sawtooth("data_grp_sawtooth_1.csv")
-- return tests
