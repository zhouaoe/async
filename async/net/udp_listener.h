/*----------------------------------------------------------------
// Copyright 2021
// All rights reserved.
//
// author: 404558965@qq.com (xiaoquanjie)
// github: https://github.com/xiaoquanjie/async
//----------------------------------------------------------------*/

#pragma once

#include <functional>
#include <unordered_map>
#include <string>

namespace net {

// 会话信息保留10分钟
class UdpListener {
public:
    struct SessionInfo {
        void* addr;
        time_t active = 0;
        SessionInfo();
        ~SessionInfo();
    };

    friend void on_cb_recv(int fd, short, void *arg);

    // 如果event_base传递空，则内部将自创建一个base
    UdpListener(void* event_base);

    virtual ~UdpListener();

    virtual void update(time_t cur_time);

    // @addr: ip:port
    bool listen(const std::string& addr);

    int send(uint32_t fd, const char* buf, uint32_t len);

    void setDataCb(std::function<void(UdpListener*, uint32_t, const char*, uint32_t)> cb);

protected:
    void expire(time_t cur_time);

protected:
    bool  m_created_base;
    void* m_event_base;
    void* m_evt;
    std::function<void(UdpListener*, uint32_t, const char*, uint32_t)> m_data_cb;
    char* m_data_buf;
    std::unordered_map<uint32_t, SessionInfo> m_fd_addr_map;
    uint32_t m_cur_poll_key;
};

}