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
 * @file msg_protocol.h
 * @brief 
 * @author Jerry.Yu (jerry.yu512outlook.com)
 * @version 1.0.0
 * @date 2022-08-24
 * 
 * @copyright MIT License
 * 
 */
#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "workflow/ProtocolMessage.h"
#include "workflow/WFTaskFactory.h"

namespace bio {
	
namespace mq {

// 传输协议(32字节)
typedef struct {
	// 魔术字
	uint32_t magic;
	// 包编号
	uint32_t seq;
	// 长度，整个包的长度(整个数据流)
	uint64_t len;
	// 校验码，checksum之后的crc32
	uint32_t checksum;
	// 版本号
	uint32_t version;
	// 正文的格式,json,xml,bin,other
	uint8_t body_format;
	// 是否是应答包
	uint8_t ack:1;
	uint8_t res1:7;
	// 是否扩展头部
	uint8_t header_extern:1;
	// 扩展头部长度(128字节)
	uint8_t header_extern_len:7;
	// 预留
	uint8_t res[5];
} msg_protocol_header_t;

// 扩展头起始数据(每个扩展头都使用该结构体)，8字节
typedef struct {
	// 扩展包类型,用来区分需要使用的扩展头结构体,0/0xffff时无效
	uint16_t type;
	// 扩展头长度(数据段，本结构体除外，最长128-8=120)
	uint8_t ext_len;
	// 有无下级扩展头(当msg_protocol_header_t中的扩展头部长度不够使用时才能使用)
	uint8_t has_next_ext: 1;
	// 下级扩展头长度(128)
	uint8_t next_ext_len: 7;
	uint8_t res[4];
} msg_protocol_ext_header_t;

// type = 1
// 消息队列扩展头,8字节。该协议的头部一共48字节
typedef struct {
	// 协议部分类型,0-json,1-xml,2-bin
	uint8_t protocl_type;
	uint8_t res[7];
	// 协议字段长度，json/xml末尾的\0要算
	uint32_t protocol_len;
} mq_msg_ext_header_t;

#if 0
{
	"version": "1.0",
	"data": {
		// uuid，表示唯一消息，req
		"uuid": "000",
		// 消息的序号，req
		"sequeue": 1,
		// 消息的时间，req
		"time": "",
		// 使用的通道url，req
		"url": "",
		// 话题，req
		"topic": "",
		// 方法，req
		"method": "subscribe",
		// 内容版本，req
		"version": "1.0",
		// QOS，opt，默认0
		"qos": 0,
		// 方法，默认为请求，响应为response，opt
		"type": "request",
		// 生存期，单位s，opt，默认无限
		"expiration": 100,
		// 优先级，opt，0-1000
		"priority": 100,
		// 消息路由时间(ttl),req
		"route": {
			// 实际发送时间,req
			"startTime": 1,
			// 中间节点时间,opt
			"node": [
				{
					"name": "xx",
					"time": 4,
				}
			],
			// 实际收到的时间,req
			"endTime": 3
		},
		// 发送者,opt
		"sender": "test",
		// 接收者,opt
		"receivers": ["ss"],
		// 压缩方法,opt
		"compress": "gzip",
		// 内容段长度，压缩后的
		"payloadLen": 200,
		// 内容段实际长度，压缩前的
		"payloadRealLen": 400,
	},
	"code": 0
	"status": "ok"
}
#endif

class TransMessageRequest: public protocol::ProtocolMessage {
private:
	virtual int encode(struct iovec vectors[], int max);
	virtual int append(const void *buf, size_t size);

public:
	int set_message_body(const void *body, size_t size);

	void get_message_body_nocopy(void **body, size_t *size)
	{
		*body = this->payload;
		*size = this->body_size;
	}

protected:
	char head[sizeof(msg_protocol_header_t)];
	size_t head_received;
	char ext_header[sizeof(msg_protocol_ext_header_t)];
	char mq_ext_header[sizeof(mq_msg_ext_header_t)];
	char *json;
	char *payload;
	size_t body_received;
	size_t body_size;

public:
	// TODO:数长度限制，内存分配接口
	TransMessageRequest() {
		this->head_received = 0;
		this->payload = NULL;
		this->body_size = 0;
	}

	TransMessageRequest(TransMessageRequest&& msg);
	TransMessageRequest& operator=(TransMessageRequest&&msg);
	virtual ~TransMessageRequest();
};

using TransMessageResponse = TransMessageRequest;

using MQTask = WFNetworkTask<TransMessageRequest, TransMessageResponse>;
	
} // namespace mq

} // namespace bio
