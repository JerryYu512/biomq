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
 * @file biomq_pubsub.h
 * @brief 
 * @author Jerry.Yu (jerry.yu512outlook.com)
 * @version 1.0.0
 * @date 2022-08-21
 * 
 * @copyright MIT License
 * 
 */
#pragma once

#include "biomq_ecode.h"
#include "biomq_msg.h"
#include <functional>

namespace bio {
	
namespace mq {

using publish_msg_cb = std::function<MsgEcode()>;
using request_msg_cb = std::function<MsgEcode(sub_msg_t&)>;
using subrecv_msg_cb = std::function<MsgEcode(sub_msg_t&)>;

class PubClientImpl;
class SubClientImpl;

class PubClient {
public:
	PubClient(const std::string& url);
	~PubClient();

public:
	// 消息推送，不关心结果
	MsgEcode publish_msg(const pub_msg_t& msg, time_t tm_ms);
	MsgEcode publish_msg_async(const pub_msg_t& msg, time_t tm_ms, publish_msg_cb cb);

	// 消息请求，带应答，关心推送结果
	// 结果通过订阅带reply
	MsgEcode request_msg(const pub_msg_t& msg, time_t tm_ms, sub_msg_t& res);
	// 异步的情况会使用一个全局订阅器
	MsgEcode request_msg_async(const pub_msg_t& msg, time_t tm_ms, request_msg_cb cb);

public:
	// 释放全局的订阅器，当有消息处于订阅时，将无法释放
	static MsgEcode release_global_subscriber(const std::string& url);
	// 设置工作数
	static MsgEcode set_global_subscriber_wokers(const std::string& url, uint32_t cnt);
	// 设置全局订阅器可订阅的消息数
	static MsgEcode set_global_subscriber_msg(const std::string& url, uint32_t cnt);

private:
	PubClientImpl *impl_;
};

class SubClient {
public:
	SubClient(const std::string& url);
	~SubClient();

public:
	// 当senders为空时接收所有来的publisher，否则只接受指定的sender
	MsgEcode subscribe(const std::string& topic, uint32_t priority, const std::vector<std::string>& senders, time_t tm_out, sub_msg_t& msg);
	// 异步的方式将使用全局订阅器进行拉取
	MsgEcode subscribe_async(const std::string& topic, uint32_t priority, const std::vector<std::string>& senders, time_t tm_out, subrecv_msg_cb cb);

public:
	// 释放全局的订阅器，当有消息处于订阅时，将无法释放
	static MsgEcode release_global_subscriber(const std::string& url);
	// 设置工作数
	static MsgEcode set_global_subscriber_wokers(const std::string& url, uint32_t cnt);
	// 设置全局订阅器可订阅的消息数
	static MsgEcode set_global_subscriber_msg(const std::string& url, uint32_t cnt);

private:
	SubClientImpl *impl_;
};

} // namespace bio

} // namespace bio
