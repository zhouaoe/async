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

class HttpListener {
public:
    friend void on_request_cb_helper(void*, void*);

    // 如果event_base传递空，则内部将自创建一个base
    HttpListener(void* event_base);

    virtual ~HttpListener();

    void* getEventBase();

    // @time_t保留
    void update(time_t);

    bool listen(const std::string& ip, uint32_t port);

    static void send(void* request, const char* buf, uint32_t len);

    static void send(void* request,
                     const std::unordered_map<std::string, std::string>& header,
                     const char* buf,
                     uint32_t len);

    // 设置接受数据的回调
    void setDataCb(std::function<void(HttpListener*, void*, const char*, uint32_t)> cb);

    // 获取对方host
    static const char* getRemoteHost(const void* request);

    // 获取路径
    static const char* getUrlPath(const void* request);

    // 获取参数
    static const char* getUrlParam(const void* request);

    static const char* decodeUri(const char* s);
private:
    bool  m_created_base;
    void* m_event_base;
    void* m_evhttp;
    std::function<void(HttpListener*, void*, const char*, uint32_t)> m_data_cb;
};

}

