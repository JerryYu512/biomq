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
 * @file biomq_log.h
 * @brief 
 * @author Jerry.Yu (jerry.yu512outlook.com)
 * @version 1.0.0
 * @date 2022-08-28
 * 
 * @copyright MIT License
 * 
 */
#pragma once
#include <functional>
#include <string>
#include <stddef.h>

namespace bio {
	
namespace mq {

// 日志等级
typedef enum {
	LOG_NON,
	LOG_DEBUG,
	LOG_TRACE,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
} LOG_LV;

using log_cb = std::function<void(LOG_LV lv, const char* filename, const char* func, int line, const char* msg)>;

/**
 * @brief 注册日志回调接口
 * 
 * @param cb 
 */
void log_redirect(log_cb cb);

class MqLog {
public:
	static void log_init(LOG_LV def_lv=LOG_INFO, bool color_en=false, log_cb cb = nullptr);
	static void set_log_level(LOG_LV lv);
};

} // namespace bio

} // namespace bio
