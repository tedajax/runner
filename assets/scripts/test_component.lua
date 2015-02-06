require("assets/scripts/component")

start = function(entity)
    counter = 100
end

local h, s, v = 0, 0, 0

function hsv_to_rgb(h, s, v)
    h = h % 360    

    local c = v * s
    local x = c * (1 - math.abs((h / 60) % 2 - 1))
    local m = v - c

    local r, g, b = 0, 0, 0

    if h < 60 then
        r, g = c, x
    elseif h < 120 then
        r, g = x, c
    elseif h < 180 then
        g, b = c, x
    elseif h < 240 then
        g, b = x, c
    elseif h < 300 then
        r, b = x, c
    else
        r, b = c, x
    end

    r = r + m
    g = g + m
    b = b + m

    return r * 255, g * 255, b * 255
end

update = function(dt)
    h = h + 60 * dt
    s = 1
    v = 1
end

render = function()
    local r, g, b = hsv_to_rgb(h, s, v)
    ffi.C.draw_rectangle_rgba(Vec2(300, 200), 100, 50, r, g, b, 255)
end
