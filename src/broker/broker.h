/**
 * MIT License
 * 
 * Copyright © 2022 <Jerry.Yu>.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS";, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * @file broker.h
 * @brief 
 * @author Jerry.Yu (jerry.yu512outlook.com)
 * @version 1.0.0
 * @date 2022-08-28
 * 
 * @copyright MIT License
 * 
 */
#pragma once

#include <map>
#include <mutex>
#include <atomic>
#include <thread>
#include "workflow/Workflow.h"
#include "workflow/WFServer.h"
#include "workflow/WFFacilities.h"
#include "biomq.h"
#include "protocol/msg_protocol.h"
#include "biomq_cached.h"

namespace bio {
	
namespace mq {
	
using MQServer = WFServer<TransMessageRequest, TransMessageResponse>;

typedef struct {
	BrokerNodeType type;
	std::string host;
	broker_node_config_t base;
	struct WFServerParams wk;
	MQServer *server;
} node_config_t;

class BrokerNodeImpl {
public:
	BrokerNodeImpl(const node_config_t& conf);

	~BrokerNodeImpl();

	int server(MQServer *server);

	static void process(BrokerNodeImpl* node, MQTask* task);
	static void wait_thread(BrokerNodeImpl* node);

private:
	WFFacilities::WaitGroup wait_group;
	node_config_t conf_;
	std::atomic_bool stoped_;
	std::atomic_bool stop_;
	std::thread th_;
	MsgCache msg_cached_;
};

class BrokerImpl {
private:
	BrokerImpl(const broker_config_t* conf);
public:
	friend class Broker;
	~BrokerImpl();

public:
	int start(void);
	int stop(void);
	MsgEcode create_node(const broker_node_config_t& conf, BrokerNode** node);

public:
	static BrokerImpl* get_instance(const broker_config_t* conf = nullptr);

private:
	// 配置
	broker_config_t conf_;
	// 节点列表
	std::map<std::string, BrokerNodeImpl*> nodes_;
	// 锁
	std::mutex mtx_;
};

} // namespace mq

} // namespace bio
