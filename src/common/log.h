#pragma once
#include "boost/type_index.hpp"
#include "spdlog/spdlog.h"

#define INFO(fmt, args...) spdlog::info(fmt, ##args)
#define ERROR(fmt, args...) spdlog::error(fmt, ##args)
#define WARN(fmt, args...) spdlog::warn(fmt, ##args)
#define FATAL(fmt, args...) spdlog::critical(fmt, ##args)

#define TypeName(type) boost::typeindex::type_id<type>().pretty_name()
