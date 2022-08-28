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
 * @file log.h
 * @brief 
 * @author Jerry.Yu (jerry.yu512outlook.com)
 * @version 1.0.0
 * @date 2022-08-28
 * 
 * @copyright MIT License
 * 
 */
#pragma once
#include "biomq_log.h"

namespace bio {
	
namespace mq {

#define CLR_CLR         "\033[0m"       /* 恢复颜色 */
#define CLR_BLACK       "\033[30m"      /* 黑色字 */
#define CLR_RED         "\033[31m"      /* 红色字 */
#define CLR_GREEN       "\033[32m"      /* 绿色字 */
#define CLR_YELLOW      "\033[33m"      /* 黄色字 */
#define CLR_BLUE        "\033[34m"      /* 蓝色字 */
#define CLR_PURPLE      "\033[35m"      /* 紫色字 */
#define CLR_SKYBLUE     "\033[36m"      /* 天蓝字 */
#define CLR_WHITE       "\033[37m"      /* 白色字 */

#define CLR_BLK_WHT     "\033[40;37m"   /* 黑底白字 */
#define CLR_RED_WHT     "\033[41;37m"   /* 红底白字 */
#define CLR_GREEN_WHT   "\033[42;37m"   /* 绿底白字 */
#define CLR_YELLOW_WHT  "\033[43;37m"   /* 黄底白字 */
#define CLR_BLUE_WHT    "\033[44;37m"   /* 蓝底白字 */
#define CLR_PURPLE_WHT  "\033[45;37m"   /* 紫底白字 */
#define CLR_SKYBLUE_WHT "\033[46;37m"   /* 天蓝底白字 */
#define CLR_WHT_BLK     "\033[47;30m"   /* 白底黑字 */

// XXX(id, str, clr)
#define LOG_LEVEL_MAP(XXX) \
    XXX(LOG_DEBUG, "DEBUG", CLR_WHITE)     \
    XXX(LOG_TRACE, "TRACE", CLR_SKYBLUE)     \
    XXX(LOG_INFO,  "INFO ", CLR_GREEN)     \
    XXX(LOG_WARN,  "WARN ", CLR_YELLOW)    \
    XXX(LOG_ERROR, "ERROR", CLR_RED)
	
// 日志为2k
#define LOG_BUF_LEN 2048

// 日志
void log_print(LOG_LV lv, const char* filename, const char* func, int line, const char* fmt, ...) __attribute__((format(printf, 5, 6)));

#define MQ_DEBUG(fmt, ...) bio::mq::log_print(bio::mq::LOG_DEBUG, __FILE__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)
#define MQ_TRACE(fmt, ...) bio::mq::log_print(bio::mq::LOG_TRACE, __FILE__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)
#define MQ_INFO(fmt, ...)  bio::mq::log_print(bio::mq::LOG_INFO,  __FILE__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)
#define MQ_WARN(fmt, ...)  bio::mq::log_print(bio::mq::LOG_WARN,  __FILE__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)
#define MQ_ERROR(fmt, ...) bio::mq::log_print(bio::mq::LOG_ERROR, __FILE__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)

} // namespace mq

} // namespace bio
