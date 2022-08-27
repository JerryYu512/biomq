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
 * @file biomq_msg.h
 * @brief 
 * @author Jerry.Yu (jerry.yu512outlook.com)
 * @version 1.0.0
 * @date 2022-08-21
 * 
 * @copyright MIT License
 * 
 */
#pragma once

#include <string>
#include <vector>
#include <map>
#include "biomq_def.h"

namespace bio {
namespace mq {

// 消息
typedef struct msg_s {
	std::string topic;
	// 消费次数
	uint32_t cust_cnt;
	// 失败次数
	uint32_t fail_cnt;
	MsgQos qos;
	uint8_t req_res;
	std::string sender;
	// 建议接收者队列
	std::vector<std::string> receivers;
	// 接收成功队列->次数
	std::map<std::string, uint32_t> recv_succ;
	// 接收失败队列->次数
	std::map<std::string, uint32_t> recv_fail;
	// 推送的时间
	msg_time_t send_time;
	// 有效期，0-未指定，数据过期时需要优先删除
	time_t expiration_date;
	// 原始长度
	uint64_t real_len;
	// 压缩方式
	uint8_t compress;
	msg_data_t payload;
} msg_t;

// 消息推送
typedef struct pub_msg_s {
	std::string topic;
	MsgQos qos;
	std::string sender;
	// 希望这些接收者能接收到
	std::vector<std::string> receivers;
	std::string payload;
	// 有效期，0-未指定
	time_t expiration_date;
} pub_msg_t;

// 消息订阅
typedef struct sub_msg_s {
	std::string topic;
	MsgQos qos;
	std::string sender;
	std::string payload;
	// 推送的时间
	msg_time_t send_time;
	// 实际收到的时间
	msg_time_t recv_time;
} sub_msg_t;

}	// mq
}	// bio