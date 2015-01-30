local ffi = require("ffi")

function split(str, delim)
	delim = delim or "%s"
	local t = {}; i = 1
	for s in string.gmatch(str, "([^"..delim.."]+)") do
		t[i] = s
		i = i + 1
	end
	return t
end

function load_api(file)
	local apif = io.open(file, "rb")
	local api = apif:read("*all")
	local lines = split(api, "\n")
	local sanitized_lines = {}
	for i, v in ipairs(lines) do
		if v:sub(1, 1) ~= "#" then
			if v:sub(1, 10) == "LUA_EXPORT" then
				local vprime = v:sub(11)
				table.insert(sanitized_lines, vprime)
			else
				table.insert(sanitized_lines, v)
			end
		end
	end
	local sanitized_api = ""
	for _, v in ipairs(sanitized_lines) do
		sanitized_api = sanitized_api .. v
	end
	return sanitized_api
end

ffi.cdef(load_api("src/scriptapi.h"))

function gettime()
	return ffi.C.get_time_ms()
end

function quit() ffi.C.quit() end
function sleep(ms) ffi.C.Sleep(ms) end

return ffi