require("assets/scripts/component")

start = function(entity)
end

update = function(dt)
end

render = function()
    r = 255
    g = 0
    b = 0
    ffi.C.draw_rectangle_rgba(Vec2(600, 200), 100, 50, r, g, b, 255)
end

