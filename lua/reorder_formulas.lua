local function sort_order(a, b)
  if ( a.order < b.order ) then
    return true
  else
    return false
  end
end

local function reorder_formulas(X) 
  local nX = 0
  for k, v in pairs(X) do
    if ( not v.dependence ) then
      v.order = 0
    end
    nX = nX + 1 
  end
  
  for i = 1, nX do 
    for k, v in pairs(X) do
      if ( not v.order ) then 
        local k2 = assert(v.dependence)
        if ( X[k2].order ) then 
          v.order = X[k2].order + 1
        end
      else
        -- print("We know the order for " .. k)
      end
    end
    local all_orders_known = true
    for k, v in pairs(X) do
      if ( not v.order ) then 
        all_orders_known = false
        break
      end
    end
    if ( all_orders_known ) then
      break
    end
  end
  --=============
  -- create a sorted version of X 
  local Y = {}
  for k, v in pairs(X) do
    v.name = k
    Y[#Y+1] = v
  end
  --=============
  table.sort(Y, sort_order)
  for k, v in ipairs(Y) do
    if ( v.dependence ) then
      local parent_idx = -1
      for k2, v2 in pairs(Y) do
        if ( v2.name == v.dependence ) then
          parent_idx = k2
          break
        end
      end
      assert(parent_idx >= 1)
      v.parent = parent_idx
    else
      v.parent = 0
    end
  end
  return Y
end
return reorder_formulas

--[[ for unit testing 

local X = {}
X.f4 = { dependence = "f3", }
X.f0 = { dependence = nil, }
X.f1 = { dependence = "f0", }
X.f3 = { dependence = "f2", }
X.f2 = { dependence = "f0", }

for k, v in pairs(X) do 
  for k2, v2 in pairs(v) do 
    print(k, k2, v2) 
  end
end
print("-------------")

local Y = reorder_formulas(X)

for k, v in ipairs(Y) do 
  print("==== " .. k .. " ======" )
  for k2, v2 in pairs(v) do 
    print(k2, v2) 
  end
end

--]]
