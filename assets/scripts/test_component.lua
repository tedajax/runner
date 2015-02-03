require("assets/scripts/component")

start = function(entity)
    counter = 100
end

local r, g, b = 0, 0, 0

update = function(dt)
    r = r + 33 * dt
    g = g + 57 * dt
    b = b + 100 * dt
end

render = function()
    ffi.C.draw_rectangle_rgba(Vec2(300, 200), 100, 50, r, g, b, 255)
end
