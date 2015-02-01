require("assets/scripts/component")

local timer = 1

update = function(dt)
    timer = timer - dt
    if timer <= 0 then
        print "alright cool"
        timer = 1
    end
end
