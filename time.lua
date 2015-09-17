
local Ltimer = require 'ltimer'

local M = {}
local timer_count = 0

local proxy
proxy = function (func)
    local f = function ()
        local st, msg = pcall(func)
        if not st then
            print('error: '..msg)
        end
        timer_count = timer_count - 1
    end
    return f
end

function M.timer_create()
    timer_count = 0
    local t = Ltimer.ltimer_systime()
    Ltimer.ltimer_create(t)
end

function M.timer_update()
    local t = Ltimer.ltimer_systime()
    Ltimer.ltimer_update(t)
end

function M.timer_gettime()
    return Ltimer.ltimer_gettime()
end

function M.timer_add(func, delay)
    timer_count = timer_count + 1
    Ltimer.ltimer_add_time(proxy(func), delay)
end

function M.timer_count()
    return timer_count
end

function M.timer_second()
    return M.timer_gettime() / 100
end

function M.usleep(i)
    Ltimer.ltimer_usleep(i)
end

return M

