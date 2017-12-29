#include "day_line.h"
bool DayLineDao::CheckExsits(const std::string &code, const std::string &day) {
  if (!conn) {
    return false;
  }
  std::string sql = "select count(*) from algo_get_hist_data where code = ? and `date` = ? ";
	TRY {
	PreparedStatement_T p = Connection_prepareStatement(conn, sql.c_str());
	PreparedStatement_setString(p, 1, code.c_str());
	PreparedStatement_setString(p, 2, day.c_str());
	ResultSet_T result = PreparedStatement_executeQuery(p);
  int count = 0;
  if (ResultSet_next(result)) {
    count = ResultSet_getInt(result, 1);
    return count >= 1;
  }
  
	} CATCH(SQLException) {
	  LOG(ERROR) << "SQLException", Exception_frame.message;
	  LOG(ERROR) << sql;
    return false;
	} 
	END_TRY;
  return false;

}

int DayLineDao::InsertOrUpdate(const std::string &code, const std::string &day, const IndictorInfo &dic_info) {
  std::string open = "0";
  std::string high = "0";
  std::string low = "0";
  std::string close = "0";
  std::string volume = "0";
  std::string amount = "0";
  dic_info.getIndic("OPEN", &open);
  dic_info.getIndic("HIGH", &high);
  dic_info.getIndic("LOW", &low);
  dic_info.getIndic("CLOSE",&close);
  dic_info.getIndic("VOLUME", &volume);
  LOG(INFO) << close;
  
  dic_info.getIndic("AMOUNT", &amount);
  std::string sql = "insert into algo_get_hist_data (code, date, open,high,close,low,volume, amount ) values(?, ?, ?, ?, ?, ?,?,? ) on duplicate key update open=?,high=?, close= ? ,low=? ,volume=? ,amount=?";
 //   for (auto &info : day_info_) {
//	std::string sql = "insert into stock_basic_choice(code, name, pe, outstanding)  values(?, ?, ?, ?) on duplicate key update name = ? , pe = ? , outstanding = ?";
	TRY {
	PreparedStatement_T p = Connection_prepareStatement(conn, sql.c_str());
	PreparedStatement_setString(p, 1, code.c_str());
	PreparedStatement_setString(p, 2, day.c_str());
	PreparedStatement_setDouble(p, 3, atof(open.c_str()));
	PreparedStatement_setDouble(p, 4, atof(high.c_str()));
	PreparedStatement_setDouble(p, 5, atof(close.c_str()));
	PreparedStatement_setDouble(p, 6, atof(low.c_str()));
	PreparedStatement_setDouble(p, 7, atof(volume.c_str()));
	PreparedStatement_setDouble(p, 8, atof(amount.c_str()));
	PreparedStatement_setDouble(p, 9, atof(open.c_str()));
	PreparedStatement_setDouble(p, 10, atof(high.c_str()));
	PreparedStatement_setDouble(p, 11, atof(close.c_str()));
	PreparedStatement_setDouble(p, 12, atof(low.c_str()));
	PreparedStatement_setDouble(p, 13, atof(volume.c_str()));
	PreparedStatement_setDouble(p, 14, atof(amount.c_str()));
	PreparedStatement_execute(p);
	} CATCH(SQLException) {
	  LOG(ERROR) << "SQLException", Exception_frame.message;
	  LOG(ERROR) << sql;
	} 
	END_TRY;
	return 0;
}

