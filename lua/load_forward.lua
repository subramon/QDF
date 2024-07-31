local plpath  = require 'pl.path'
local pldir  = require 'pl.dir'
local sh_exec = require 'sh_exec'
local chk_data_files_exist = require 'chk_data_files_exist'
local wipe_dirs = require 'wipe_dirs'
local function load_forward(root_dir, src_dir, dst_dir)
  assert(type(src_dir) == "string"); assert(plpath.isdir(src_dir))
  assert(type(dst_dir) == "string"); assert(plpath.isdir(dst_dir))
  assert(src_dir ~= dst_dir)
  -- Load forward data 
  local fwd_data = src_dir .. "/fwd_data_files.lua"
  -- confirm files listed in fwd_data exist
  assert(chk_data_files_exist(fwd_data))
  assert(pldir.copyfile(fwd_data, dst_dir .. "/data_files.lua"))
  
  local fwd_save = src_dir .. "/fwd_save_info.lua"
  -- clear out files that exist in directories specified in fwd_save_info
  assert(wipe_dirs(fwd_save))
  assert(pldir.copyfile(fwd_save, dst_dir .. "/save_info.lua"))
  pldir.copyfile(src_dir .. "in_meta.lua", dst_dir)
  -- ready to load forward data 
  cmd = "cd ~/QDF/src/; lightR_load " .. root_dir .." main; cd -; pwd"
  local rslt = assert(sh_exec(cmd))
  print(cmd)
  return true
end
return load_forward
