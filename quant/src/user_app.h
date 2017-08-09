#ifndef USER_APP_H
#define USER_APP_H
#include <string>
#include <list>
struct wrapper_Info;
int user_recv(const std::list<std::string> &messages);
void set_wrapper_info(wrapper_Info *info);
#endif
