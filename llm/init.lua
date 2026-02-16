local cutils = require 'libcutils'

local O = {} -- operators listed for QDF

local C = require 'coalesce'
assert(type(C) == "table")
require('register').register('coalesce', C)

return require 'register'

