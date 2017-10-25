#ifndef USER_APP_H
#define USER_APP_H
#include <string>
#include <list>
#include "stock_info.h"
struct wrapper_Info;
int user_recv(const std::map<std::string, CsqInfo> &messages);
void set_wrapper_info(wrapper_Info *info);
#endif
