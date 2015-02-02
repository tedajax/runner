require("assets/scripts/component")

local timer = 1
local counter = 0

start = function(entity)
    counter = 100
end

update = function(dt)
    timer = timer - dt
    if timer <= 0 then
        print("alright cool stuff "..counter)
        counter = counter + 10
        timer = 0.1
    end
end
