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
 * @file biomq_log.cpp
 * @brief 
 * @author Jerry.Yu (jerry.yu512outlook.com)
 * @version 1.0.0
 * @date 2022-08-28
 * 
 * @copyright MIT License
 * 
 */
#include "log.h"
#include <sstream>
#include <time.h>
#include <sys/time.h>

namespace bio {
	
namespace mq {

static log_cb mq_log = nullptr;

typedef struct {
	LOG_LV lv;
	bool color;
	log_cb cb;
} log_config_t;

static log_config_t log_conf = {
	LOG_INFO,
	false,
	nullptr,
};
	
void log_redirect(log_cb cb) {
	mq_log = cb;
}

void MqLog::log_init(LOG_LV def_lv, bool color_en, log_cb cb) {
	log_conf.lv = def_lv;
	log_conf.color = color_en;
	log_conf.cb = cb;
}

void MqLog::set_log_level(LOG_LV lv) {
	log_conf.lv = lv;
}

void log_print(LOG_LV lv, const char* filename, const char* func, int line, const char* fmt, ...) {
	// 无日志
	if (lv < log_conf.lv || LOG_NON == log_conf.lv) {
		return;
	}

	int year,month,day,hour,min,sec,us;
	char buf[LOG_BUF_LEN] = "";
	int bufsize = LOG_BUF_LEN;
	int len = 0;

	struct timeval tv;
    struct tm* tm = NULL;
    gettimeofday(&tv, NULL);
    time_t tt = tv.tv_sec;
    tm = localtime(&tt);
    year     = tm->tm_year + 1900;
    month    = tm->tm_mon  + 1;
    day      = tm->tm_mday;
    hour     = tm->tm_hour;
    min      = tm->tm_min;
    sec      = tm->tm_sec;
    us       = tv.tv_usec;

	const char* pcolor = "";
    const char* plevel = "";
#define XXX(id, str, clr) \
    case id: plevel = str; pcolor = clr; break;

    switch (lv) {
        LOG_LEVEL_MAP(XXX)
    }
#undef XXX

	// 颜色
	if (log_conf.color) {
		len = snprintf(buf, bufsize, "%s", pcolor);
	}

	// 时间
	len += snprintf(buf + len, bufsize - len, "%04d-%02d-%02d %02d:%02d:%02d.%03d %s [BIOMQ] ",
            year, month, day, hour, min, sec, us/1000,
            plevel);
	
	if (!log_conf.cb) {
		len += snprintf(buf + len, bufsize - len, "[%s:%d][%s] ", filename, line, func);
	}

	// 日志
	va_list ap;
	va_start(ap, fmt);
	len += vsnprintf(buf + len, bufsize - len, fmt, ap);
	va_end(ap);

	if (log_conf.color) {
        len += snprintf(buf + len, bufsize - len, "%s", CLR_CLR);
    }

	// 重定向
	if (log_conf.cb) {
		log_conf.cb(lv, filename, func, line, buf);
	} else {
		printf("%s", buf);
	}
}

} // namespace mq

} // namespace bio
