#ifndef __tarcza_H__
#define __tarcza_H__

#include <tizen.h>
#include <watch_app.h>
#include <watch_app_efl.h>
#include <Elementary.h>
#include <dlog.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "tarcza"

#if !defined(PACKAGE)
#define PACKAGE "re.cvgo.tzn_tarcza"
#endif

#define DBG_ERR(...) dlog_print(DLOG_ERROR, LOG_TAG, __VA_ARGS__);

#endif /* __tarcza_H__ */
