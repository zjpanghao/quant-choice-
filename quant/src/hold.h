#ifndef INCLUDE_HOLD_H
#define INCLUDE_HOLD_H
#include "indictor_info.h"
#include "glog/logging.h"
#include "dbpool.h"
class HoldDao;
class HoldDao {
 public:
  HoldDao(DBPool *pool) : conn(NULL) {
   if (pool) {
	   conn = pool->GetConnection();
	   if (!conn) {
		   LOG(ERROR) << "NULL connection";
		 } 
	 } 

  }

  int InsertOrUpdate(const IndictorInfo &dic_info); 
  ~HoldDao() {
   if (conn)
	   Connection_close(conn);
  }

 private:
   Connection_T  conn;
};

#endif
