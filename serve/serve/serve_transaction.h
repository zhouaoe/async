/*----------------------------------------------------------------
// Copyright 2022
// All rights reserved.
//
// author: 404558965@qq.com (xiaoquanjie)
//----------------------------------------------------------------*/

#pragma once

#include "common/transaction/base_transaction.h"
#include "serve/serve/base.h"
#include "serve/serve/backend.h"
#include <assert.h>

template<typename RequestType, typename RespondType=NullRespond>
class ServeTransaction : public Transaction<RequestType, RespondType> {
public:
    using BaseTrans = Transaction<RequestType, RespondType>;

    bool ParsePacket(const char* packet, uint32_t /*packet_size*/) override {
        const BackendMsg* msg = (const BackendMsg*)packet;
        if (!BaseTrans::m_request.ParseFromString(msg->data)) {
            assert(false);
            return false;
        }

        this->mheader = msg->header;
        return true;
    }

    bool OnBefore() override {
        return true;
    }

    bool OnAfter() override {
        // 如何判别是单向还是双向
        // if (typeid(RespondType).name() != typeid(NullRespond).name()) {
        //     // 回包

        // }
        return true;
    }



protected:
    BackendHeader mheader;
};

