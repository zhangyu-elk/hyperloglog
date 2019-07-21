local hll = require "hll"

local hdr = hll.new()
for i = 1, 1000 do
	local s = tostring(i)
	hdr:add(s, #s)
end
print(hdr:count())

for i = 1, 1000 do
	local s = tostring(i)
	hdr:add(s, #s)
end
print(hdr:count())

for i = 1000, 2000 do
	local s = tostring(i)
	hdr:add(s, #s)
end
print(hdr:count())

for i = 2000, 8000 do
	local s = tostring(i)
	hdr:add(s, #s)
end
print(hdr:count())


local hdr2 = hll.new()
for i = 2000, 8000 do
	local s = tostring(i)
	hdr2:add(s, #s)
end
print(hdr2:count())

print(hdr:merge(hdr2))
print(hdr:count())