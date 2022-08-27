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
 * @file biomq_cached.h
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
#include <vector>
#include <queue>
#include <map>
#include "biomq_def.h"
#include "biomq_ecode.h"
#include "biomq_msg.h"

namespace bio {
	
namespace mq {

typedef struct msg_list_s {
	msg_list_s *prev;
	msg_list_s *next;
} msg_list_t;

// 双向链表消息
typedef struct {
	msg_list_t node;
	msg_t msg;
} msg_data_cached_t;

// 该链表使用过期时间排序，无时间的直接插入末尾，最快要过期的放在前方
typedef struct {
	// 队列节点
	msg_list_t node;
	// 记录的数据节点
	msg_list_t *data;
} msg_data_time_series_t;

// topic->msg_list node
using MsgCachedMap = std::map<std::string, std::queue<msg_list_t*>>;

typedef struct {
	uint64_t max_topic;
	uint64_t max_msg;
	uint64_t max_mem;
} msg_cache_config_t;

class MsgCache {
public:
	MsgCache(const msg_cache_config_t& conf);
	~MsgCache();

public:
	// 统计接口
	uint64_t msg_cnt(const std::string& topic) const;
	uint64_t msg_cnt(void) const;
	uint64_t topic_cnt(void) const;
	uint64_t mem_cnt(void) const;

public:
	// 存消息
	MsgEcode push(const msg_data_cached_t& msg);
	// 取消息
	MsgEcode pop(msg_data_cached_t& msg);
	// 垃圾回收，清理过期的数据
	void gc(void);

private:
	// topic映射
	MsgCachedMap map_;
	// 缓存队列
	msg_data_cached_t msg_;
	// 时间序列
	msg_data_time_series_t time_series_;

	// 消息条数计数
	uint64_t msg_cnt_;
	// 话题计数
	uint64_t topic_cnt_;
	// 内存计数
	uint64_t mem_cnt_;
	msg_cache_config_t config_;
};
	
} // namespace mq

} // namespace bio
