#include "hold.h"
int HoldDao::InsertOrUpdate(const IndictorInfo &dic_info) {
  std::string code = "--";
  std::string instrumentName = "--";
  std::string noticeDate = "";
  std::string actionDate = "";
  std::string shareName = "--";
  std::string changeNum = "0";
  std::string changeRate = "0";
  std::string numAfterChange = "0";
  std::string rateAfterChange = "0";
  std::string price = "0";
  std::string zjhName = "--";
  std::string eastName = "--";
  int increase = 0;
  std::string table;
  dic_info.getIndic("SECURITYCODE", &code);
  if (code.length() > 6) {
    code = code.substr(0, 6);
  }
  dic_info.getIndic("SECURITYSHORTNAME", &instrumentName);
  dic_info.getIndic("NOTICEDATE", &noticeDate);
  dic_info.getIndic("BDJZRQ",&actionDate);
  dic_info.getIndic("SHAREHDNAME",&shareName);
  dic_info.getIndic("CHANGENUM", &changeNum);
  dic_info.getIndic("BDSLZLTB", &changeRate);
  dic_info.getIndic("BDHCYLTGSL", &numAfterChange);
  dic_info.getIndic("BDHCYLTSLZLTGB", &rateAfterChange);
  dic_info.getIndic("JYPJJ", &price);
  dic_info.getIndic("table", &table);
  if (table == "HoldIncreaseInfo") {
    increase = 1;
  }
  
  std::string sql = "insert into hold_change_info (instrument_code, instrument_name, notice_date, action_date, share_name, change_num, change_rate, num_after_change,rate_after_change, price, action) values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?,?)";
 //   for (auto &info : day_info_) {
//	std::string sql = "insert into stock_basic_choice(code, name, pe, outstanding)  values(?, ?, ?, ?) on duplicate key update name = ? , pe = ? , outstanding = ?";
	TRY {
  Connection_executeQuery(conn, "set names utf8");
	PreparedStatement_T p = Connection_prepareStatement(conn, sql.c_str());
	PreparedStatement_setString(p, 1, code.c_str());
	PreparedStatement_setString(p, 2, instrumentName.c_str());
	PreparedStatement_setString(p, 3, noticeDate.c_str());
	PreparedStatement_setString(p, 4, actionDate.c_str());
	PreparedStatement_setString(p, 5, shareName.c_str());
	PreparedStatement_setInt(p, 6, (int)(atof(changeNum.c_str()) * 10000));
	PreparedStatement_setDouble(p, 7, atof(changeRate.c_str()));
	PreparedStatement_setInt(p, 8, (int)(atof(numAfterChange.c_str()) * 10000));
	PreparedStatement_setDouble(p, 9, atof(rateAfterChange.c_str()));
	PreparedStatement_setDouble(p, 10, atof(price.c_str()));
	PreparedStatement_setInt(p, 11, increase);
  LOG(INFO) << code;
	PreparedStatement_execute(p);
	} CATCH(SQLException) {
	  LOG(ERROR) << "SQLException", Exception_frame.message;
	  LOG(ERROR) << sql;
	} 
	END_TRY;
	return 0;
}

