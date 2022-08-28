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
 * @file broker.cpp
 * @brief 
 * @author Jerry.Yu (jerry.yu512outlook.com)
 * @version 1.0.0
 * @date 2022-08-27
 * 
 * @copyright MIT License
 * 
 */
#include <string.h>
#include <memory>
#include <workflow/MD5Util.h>
#include "common/log.h"
#include "broker.h"

namespace bio {
	
namespace mq {

BrokerNodeImpl::BrokerNodeImpl(const node_config_t& conf)
: wait_group(1),
  conf_(conf),
  stoped_(true),
  stop_(false),
  msg_cached_((msg_cache_config_t){
				conf.base.max_topic,
				1024*10,
				conf.base.max_mem}) {

}

BrokerNodeImpl::~BrokerNodeImpl() {
	// if (!stoped_) {
	// 	uint32_t cnt = 0;
	// 	for (; !stop_; cnt++) {
	// 		if (cnt >= 100) {
	// 			MQ_WARN("broker nonde[%s] can not exit.", conf_.base.url);
	// 		}
	// 		for (uint16_t i = 0; i < 10000; i++);
	// 	}
	// }

	MQ_INFO("broker nonde[%s] exiting.", conf_.base.url);

	stop_ = true;
	th_.join();

	MQ_INFO("broker nonde[%s] exited.", conf_.base.url);

	if (conf_.server) {
		// TODO:如果外部触发退出
		// conf_.server->shutdown();
		// wait_group.wait();
		// conf_.server->stop();
		delete conf_.server;
		conf_.server = nullptr;
	}
}

int BrokerNodeImpl::server(MQServer *server) {
	if (!conf_.server) {
		conf_.server = server;
		th_ = std::thread(wait_group, this);
	}
	return MSG_ECODE_OK;
}

void BrokerNodeImpl::process(BrokerNodeImpl* node, MQTask* task) {
	// TODO:
	// 消息订阅
	// 消息获取
	// 系统消息
}

void BrokerNodeImpl::wait_thread(BrokerNodeImpl* node) {
	// TODO:如果外部触发退出
	// node->stoped_ = false;
	// node->stop_ = false;
	node->wait_group.wait();
	node->conf_.server->stop();
}

BrokerImpl::BrokerImpl(const broker_config_t* conf)
	: conf_(*conf) {

}

BrokerImpl::~BrokerImpl() {
	for (auto & item : nodes_) {
		delete item.second;
		item.second = nullptr;
	}
}

int BrokerImpl::start(void) {

}
int BrokerImpl::stop(void) {

}
MsgEcode BrokerImpl::create_node(const broker_node_config_t& conf, BrokerNode** node) {
	std::lock_guard<std::mutex> lck(mtx_);
	MQ_DEBUG("node size = %zu", nodes_.size());
	if (nodes_.size() >= conf_.max_node) {
		return MSG_ECODE_NODE_OVER_MAX;
	}

	if (!node) {
		MQ_ERROR("node is nullptr.");
		return MSG_ECODE_INVALID_MEM;
	}

	if (nodes_.find(conf.url) != nodes_.end()) {
		MQ_TRACE("node exist: %s", conf.url);
		return MSG_ECODE_NODE_EXISTED;
	}

	// 取地址
	node_config_t node_conf;
	struct WFServerParams wk = SERVER_PARAMS_DEFAULT;
	BrokerNodeType type = BROKER_NET;
	const char* host = strstr(conf.url.c_str(), "ipc://");
	// const char* ip = nullptr;
	std::string ip;
	unsigned short port = 0;

	MQ_DEBUG("usl: %s", conf.url.c_str());
	if (!host) {
		host = strstr(conf.url.c_str(), "net://");
		if (!host) {
			MQ_TRACE("not find \"net://\"");
			return MSG_ECODE_NET_URL_ERROR;
		} else {
			type = BROKER_NET;
			host = host + strlen("net://");
		}
	} else {
		type = BROKER_IPC;
		host = host + strlen("ipc://");
	}

	const char* ptr = strstr(host, ":");
	if (!ptr) {
		MQ_TRACE("not find host:port");
		return MSG_ECODE_NET_URL_ERROR;
	}

	// 取出ip地址
	ip = std::string(host, ptr - host);
	ptr = ptr + strlen(":");
	// 取出端口
	int tmp_port = atoi(ptr);

	if (tmp_port <= 0 || tmp_port >= UINT16_MAX) {
		MQ_TRACE("port not in range");
		return MSG_ECODE_NET_URL_ERROR;
	}

	port = tmp_port;

	// 校验url格式是否正确
	// 校验其他参数
	// url是否存在

	node_conf.base = conf;
	node_conf.host = host;
	node_conf.type = type;
	node_conf.wk = wk;
	node_conf.wk.request_size_limit = node_conf.base.max_msg_len;

	MQ_DEBUG("node config:\n"
			"base.url:%s\n"
			"base.max_topic:%d\n"
			"base.max_mem:%d\n"
			"base.max_wokers:%d\n"
			"base.max_msg_len:%d\n"
			"base.min_msg_compress_len:%d\n"
			"tpye:%s\n",
			node_conf.base.url,
			node_conf.base.max_topic,
			node_conf.base.max_mem,
			node_conf.base.max_wokers,
			node_conf.base.max_msg_len,
			node_conf.base.min_msg_compress_len,
			BROKER_IPC == type ? "ipc" : "net");

	// 创建节点
	BrokerNodeImpl *new_node = new BrokerNodeImpl(node_conf);
	// 创建服务
	node_conf.server = new MQServer(&node_conf.wk, std::bind(BrokerNodeImpl::process, new_node, std::placeholders::_1));
	int ret = node_conf.server->start(ip.c_str(), port);
	if (0 != ret) {
		delete node_conf.server;
		delete new_node;
		MQ_ERROR("start wk server error:%d", ret);
		return MSG_ECODE_NODE_LITSEN;
	}
	// 保存服务指针
	new_node->server(node_conf.server);
	MQ_INFO("broker node on %s ...", host);

	// 加入管理
	nodes_.insert(std::pair<std::string, BrokerNodeImpl*>(node_conf.base.url, new_node));

	return MSG_ECODE_OK;
}

BrokerImpl* BrokerImpl::get_instance(const broker_config_t* conf = nullptr)
{
	static BrokerImpl impl(conf);

	return &impl;
}


// 代理接口
Broker::Broker(const broker_config_t& conf) {
	impl_ = BrokerImpl::get_instance(&conf);
}

Broker::~Broker() {
	impl_->stop();
	impl_ = nullptr;
}

int Broker::start(void) {
	return impl_->start();
}

int Broker::stop(void) {
	return impl_->stop();
}

MsgEcode Broker::create_node(const broker_node_config_t& conf, BrokerNode** node) {
	return impl_->create_node(conf, node);
}

void hello_broker() {

}
	
} // namespace bio

} // namespace bio
