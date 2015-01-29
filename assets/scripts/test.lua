local ffi = require("ffi")

ffi.cdef[[
	void Sleep(int ms);
	void test_lua();
	void quit();
]]

function sleep(s)
	ffi.C.Sleep(s)
end

function test()
	ffi.C.test_lua()
end

for i = 1, 25 do
	io.write(".")
	io.flush()
end
io.write("\n")
test()
sleep(2000)
ffi.C.quit()