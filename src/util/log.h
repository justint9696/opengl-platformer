/**
 * @file log.h
 * @author Justin Tonkinson
 * @data 2024/10/18
 * @brief This file contains function prototypes and macros for custom logging.
 */

#ifndef _UTIL_LOG_H_
#define _UTIL_LOG_H_

enum {
    LOG_FATAL,
    LOG_ERROR,
    LOG_WARN,
    LOG_DEBUG,
    LOG_MAX,
};

/**
 * @brief Logs a custom message.
 *
 * This function depends on the `_DEBUG` macro to log terminal messages. If
 * the macro is undefined, a copy of the log can be found at `./game.log`.
 *
 * @note This function is intended for internal use only.
 */
void _log_message(int level, const char *file, const char *function, int line,
                  const char *format, ...);

#define log_debug(_message, ...) \
    _log_message(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, _message, \
                ##__VA_ARGS__)

#define log_warn(_message, ...) \
    _log_message(LOG_WARN, __FILE__, __FUNCTION__, __LINE__, _message, \
                ##__VA_ARGS__)

#define log_fatal(_message, ...) \
    _log_message(LOG_FATAL, __FILE__, __FUNCTION__, __LINE__, _message, \
                ##__VA_ARGS__)

#define log_error(_message, ...) \
    _log_message(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__, _message, \
                ##__VA_ARGS__)

#endif // ifndef _UTIL_LOG_H_
