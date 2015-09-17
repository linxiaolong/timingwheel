
local Time = require 'time'

local function logic()
    local delay = math.random(1, 10) * 100
    local func
    func = function ()
        local i = 1
    end
    Time.timer_add(func, delay)
end

local function main()
    Time.timer_create()
    
    for i = 1, 1000000 do
        logic()
    end
    while 1 do
        print(Time.timer_count())
        Time.timer_update()
        Time.usleep(2500)
    end
end

main()

