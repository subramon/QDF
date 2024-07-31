local trim_qtype = require 'trim_qtype'
local qtypes     = require 'qtypes'
local time_format_from_qtype = require 'time_format_from_qtype'
local function chck_meta(M)
  assert(type(M) == "table")
  assert(#M >= 1)
  local n_to_load = 0
  for i, m in ipairs(M) do
    assert(type(m) == "table")
    local qtype = m.qtype
    local name1 = m.name
    assert(type(name1) == "string")
    assert(#name1 > 0)
    qtype = trim_qtype(qtype)
    assert(qtypes[qtype])
    if ( qtype == "TM" ) then 
      local fmt = time_format_from_qtype(m.qtype)
      if ( fmt ) then 
        assert(( fmt == "%Y-%m-%d") or (fmt == "%m/%d/%Y"))
        -- add more as you choose to support
      end
    end

    for j, m2 in ipairs(M) do
      local name2 = m.name
      if ( i ~= j ) then assert(m ~= m2) end
    end

    if ( type(m.has_nulls) ~= "nil" ) then 
      assert(type(m.has_nulls) == "boolean")
    else
      m.has_nulls = true
    end

    if ( type(m.must_load) ~= "nil" ) then 
      assert(type(m.must_load) == "boolean")
    else
      m.must_load = false
    end
    if ( type(m.is_load) ~= "nil" ) then 
      assert(type(m.is_load) == "boolean")
    else
      m.is_load = true
    end
    if ( m.is_load ) then 
      n_to_load = n_to_load + 1
    end
    if ( m.is_load ) then
      assert(qtype ~= "SV") -- TODO not yet implemented
    end
  end
  assert(n_to_load > 0)
  return true
end
return chck_meta
