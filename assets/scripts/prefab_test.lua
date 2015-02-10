require("assets/scripts/component")

start = function(entity)
    entity = entity
    a = 0
end

update = function(dt)
    a = a + 2 * dt
end

render = function()
    r = 255
    g = 127
    b = 0
    
    ffi.C.draw_rectangle_rgba(Vec2(640, 360), 1280, 720, r, g, b, ((math.sin(a) + 1) / 2) * 0.5 * 255)
end

