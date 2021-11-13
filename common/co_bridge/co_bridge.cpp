#include "common/co_bridge/co_bridge.h"
#include "common/co_bridge/time_pool.h"
#include "common/coroutine/coroutine.hpp"
#include "common/async/async.h"
#include <unordered_set>
#include <unordered_map>
#include <assert.h>

namespace co_bridge { 

struct sequence_info {
    int64_t timer_id = 0;
    unsigned int co_id = 0;
    void* extra = 0;
};

int64_t g_unique_id = 1;
std::unordered_set<int64_t> g_unique_id_set;
std::unordered_map<int64_t, sequence_info> g_sequence_map;
TimerPool g_time_pool;

int wait(uint32_t interval) {
    unsigned int co_id = Coroutine::curid();
    if (co_id == M_MAIN_COROUTINE_ID) {
        assert(false);
        return co_bridge::E_CO_RETURN_ERROR;
    }

    co_bridge::addTimer(interval, [co_id]() {
        Coroutine::resume(co_id);
    });

    Coroutine::yield();
    return co_bridge::E_CO_RETURN_OK;
}

int64_t genUniqueId() {
    return g_unique_id++;
}

void addUniqueId(int64_t id) {
    assert(g_unique_id_set.find(id) == g_unique_id_set.end());
    g_unique_id_set.insert(id);
}

bool rmUniqueId(int64_t id) {
    auto iter = g_unique_id_set.find(id);
    if (iter != g_unique_id_set.end()) {
        g_unique_id_set.erase(iter);
        return true;
    }
    return false;
}

int64_t genSequenceId() {
    return g_unique_id++;
}

void addSequenceId(int64_t id, int64_t timer_id, unsigned int co_id, void* extra) {
    assert(g_sequence_map.find(id) == g_sequence_map.end());
    auto pair = g_sequence_map.insert(std::make_pair(id, sequence_info()));
    auto iter = pair.first;
    iter->second.timer_id = timer_id;
    iter->second.co_id = co_id;
    iter->second.extra = extra;
}

bool rmSequenceId(int64_t id, int64_t& timer_id, unsigned int& co_id, void** extra) {
    auto iter = g_sequence_map.find(id);
    if (iter != g_sequence_map.end()) {
        timer_id = iter->second.timer_id;
        co_id = iter->second.co_id;
        *extra = iter->second.extra;
        g_sequence_map.erase(iter);
        return true;
    }
    return false;
}

bool rmSequenceId(int64_t id) {
    auto iter = g_sequence_map.find(id);
    if (iter != g_sequence_map.end()) {
        g_sequence_map.erase(iter);
        return true;
    }
    return false;
}

int64_t addTimer(int interval, std::function<void()> func) {
    return g_time_pool.AddTimer(interval, func);
}

void rmTimer(int64_t timer_id) {
    g_time_pool.CancelTimer(timer_id);
}

bool loop() {
    g_time_pool.Update();
    return async::loop();
}

}