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
 * @file biomq.h
 * @brief 
 * @author Jerry.Yu (jerry.yu512outlook.com)
 * @version 1.0.0
 * @date 2022-08-21
 * 
 * @copyright MIT License
 * 
 */
#pragma once
#include <stdint.h>
#include <string>
#include <functional>
#include "biomq_msg.h"
#include "biomq_ecode.h"

#define BIOMQ_MAX_TOPIC_LEN 256
#define BIOMQ_MAX_TOPIC_CNT 2048
#define BIOMQ_MIN_NODE_CNT 1
#define BIOMQ_DEF_WOKERS 5
#define BIOMQ_MIN_WOKERS 1
#define BIOMQ_MAX_WOKERS 16
// 默认消息最长为256KB
#define BIOMQ_DEF_MSG_LEN (256 * 1024 * 1024)
// 默认超过4KB的数据就压缩
#define BIOMQ_DEF_MSG_COMPRESS_LEN (4 * 1024 * 1024)

namespace bio {
	
namespace mq {

// broker配置
typedef struct {
	// 最大服务数量，1-->
	uint32_t max_node;
} broker_config_t;

using mq_topic_midware_cb = std::function<void(std::string& topic, const std::string& sender, const std::vector<std::string>& receiver)>;
using mq_msg_midware_cb = std::function<void(msg_t&)>;
using mq_msg_pub_midware_cb = std::function<void(msg_t&, std::string&receiver, uint32_t result)>;
using mq_msg_gc_midware_cb = std::function<void(msg_t&, uint32_t reason)>;

// broker中间件，用于维护用，但是中间件过多会影响效率
typedef struct {
	// 有订阅
	std::vector<mq_topic_midware_cb> msg_sub;
	// 有取消订阅
	std::vector<mq_topic_midware_cb> msg_unsub;
	// 推送消息到来
	std::vector<mq_msg_midware_cb> msg_in;
	// 消息推送,当次的接收者,结果
	std::vector<mq_msg_pub_midware_cb> msg_pub_succ;
	// 消息过期
	std::vector<mq_msg_midware_cb> msg_timeout;
	// 消息丢弃,丢弃原因
	std::vector<mq_msg_gc_midware_cb> msg_trush;
} broker_node_midwares;

// 单个broker节点配置
typedef struct {
	// 同进程内local://
	// 进程间ipc://
	// 网络net://
	std::string url;
	// 支持发布订阅的最大话题数
	uint32_t max_topic;
	// 支持的最大内存(仅消息)
	uint32_t max_mem;
	// 最大工作数，设备性能好可以用多一点
	uint32_t max_wokers;
	// 最大消息长度
	uint32_t max_msg_len;
	// 最小数据压缩长度（超过开始压缩）
	uint32_t min_msg_compress_len;
	// 中间件
	broker_node_midwares midwares;
} broker_node_config_t;

class BrokerNodeImpl;
class BrokerImpl;

// broker节点
class BrokerNode {
public:
	BrokerNode(const broker_node_config_t& conf);
	~BrokerNode();
private:
	BrokerNodeImpl *impl_;
};

// broker
class Broker {
public:
	Broker(const broker_config_t& conf);
	~Broker();
public:
	MsgEcode create_node(const broker_node_config_t& conf, BrokerNode** node);

private:
	BrokerImpl *impl_;
};
	
} // namespace mq

} // namespace bio
