#ifndef INCLUDE_DAY_LINE_H
#define INCLUDE_DAY_LINE_H
#include "indictor_info.h"
#include "glog/logging.h"
#include "dbpool.h"
class DayLineDao;
class DayLineDao {
 public:
  DayLineDao(DBPool *pool) : conn(NULL) {
   if (pool) {
	   conn = pool->GetConnection();
	   if (!conn) {
		   LOG(ERROR) << "NULL connection";
		 } 
	 } 

  }

  bool CheckExsits(const std::string &code, const std::string &day); 

  int InsertOrUpdate(const std::string &code, const std::string &day, const IndictorInfo &dic_info); 
  ~DayLineDao() {
   if (conn)
	   Connection_close(conn);
  }

 private:
   Connection_T  conn;
};

#endif
