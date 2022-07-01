#ifdef USE_ASYNC_CURL

#include "./common.h"

extern bool print_log;

// http测试
void curl_test(bool use_co) {
    // 使用协程
    if (use_co) {
        // 启动一个协程任务
        CoroutineTask::doTask([](void*) {
            // 执行http访问并等待协程返回
            {
                auto res = co_async::curl::get("http://baidu.com");
                if (co_async::checkOk(res)) {
                    printf("1、curlCode:%d, resCode:%d, bodySize:%ld\n", res.second->curlCode, res.second->resCode, res.second->body.size());
                }
                if (co_async::checkTimeout(res)) {
                    printf("1、timeout\n");
                }
            }

            {
                std::string body;
                auto ret = co_async::curl::post("http://192.168.0.160/user/showEmail", "", body);
                if (ret == co_async::E_OK) {
                    printf("2、bodySize:%ld\n", body.size());
                }
                if (ret == co_async::E_TIMEOUT) {
                    printf("2、timeout\n");
                }
            }

            {
                auto res = co_async::curl::post("http://192.168.0.160/user/showEmail", "");
                if (co_async::checkOk(res)) {
                    printf("3、curlCode:%d, resCode:%d, bodySize:%ld\n", res.second->curlCode, res.second->resCode, res.second->body.size());
                }
                if (co_async::checkTimeout(res)) {
                    printf("3、timeout\n");
                }
            }

        }, 0);
    }
    // 不使用协程
    else {
        // 发起一个http异步访问并期待回调被调用
        async::curl::get("http://baidu.com", [](int, int, const std::string& body) {
            std::cout << "curl body.len: " << body.length() << std::endl;
        });
    }
}

#endif