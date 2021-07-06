#pragma once

#include <boxer/boxer.h>

#define MW_ERROR_POPUP(message, title) 	boxer::show(std::string(message).c_str(), std::string(title).c_str(), boxer::Style::Error)
#define MW_INFO_POPUP(message, title) 	boxer::show(std::string(message).c_str(), std::string(title).c_str(), boxer::Style::Info)
#define MW_WARN_POPUP(message, title) 	boxer::show(std::string(message).c_str(), std::string(title).c_str(), boxer::Style::Warning)