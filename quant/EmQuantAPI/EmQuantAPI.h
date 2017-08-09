/************************************************************************
*                                                                       *
*   EmQuantAPI.h    version  1.0.3.0                                    *
*                                                                       *
*   Copyright (c) EastMoney Corp. All rights reserved.                  *
*                                                                       *
************************************************************************/
#ifndef _EMQUANTAPI_H_
#define _EMQUANTAPI_H_ 

#include <stdint.h>

#ifdef WIN32
#ifdef EMQUANTAPI_EXPORTS
#define EMQUANTAPI  extern "C" __declspec(dllexport)
#else
#define EMQUANTAPI extern "C" __declspec(dllimport)
#endif /*#ifdef EMQUANTAPI_EXPORTS*/
#else
#define EMQUANTAPI extern "C"
#endif /*#ifdef WIN32*/

//错误码的类型定义
typedef int EQErr;

//请求ID的类型定义
typedef int EQID;

#ifndef WIN32
typedef void* LPVOID;
#endif //#ifndef WIN32
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//成功
#define EQERR_SUCCESS                 (0)
//错误基数                            
#define EQERR_BASE                    (10000000)
//一般性错误                          
#define EQERR_BASE_GENERAL            (EQERR_BASE)
//账户相关错误
#define EQERR_BASE_ACCOUT             (EQERR_BASE + 1000)
//网络相关错误                        
#define EQERR_BASE_NET                (EQERR_BASE + 2000)
//参数或请求错误                      
#define EQERR_BASE_PARAM              (EQERR_BASE + 3000)


#pragma region 账户相关错误 
//用户未登陆
#define EQERR_NO_LOGIN                (EQERR_BASE_ACCOUT + 1)
//用户名或密码错误
#define EQERR_USERNAMEORPASSWORD_ERR  (EQERR_BASE_ACCOUT + 2)
//用户无API权限
#define EQERR_NO_ACCESS               (EQERR_BASE_ACCOUT + 3)
//用户API权限过期
#define EQERR_ACCESS_EXPIRE           (EQERR_BASE_ACCOUT + 4)
//获取用户信息失败
#define EQERR_GETUSERINFO_FAIL        (EQERR_BASE_ACCOUT + 5)
//DLL版本号过期
#define EQERR_DLLVESION_EXPIRE        (EQERR_BASE_ACCOUT + 6)
//用户无API_LV2权限
#define EQERR_NO_LV2_ACCESS           (EQERR_BASE_ACCOUT + 7)
//用户API_LV2权限过期
#define EQERR_LV2_ACCESS_EXPIRE       (EQERR_BASE_ACCOUT + 8)
//账号登陆数达到上限
#define EQERR_LOGIN_COUNT_LIMIT       (EQERR_BASE_ACCOUT + 9)
//用户登陆失败
#define EQERR_LOGIN_FAIL              (EQERR_BASE_ACCOUT + 10)
//用户登陆掉线
#define EQERR_LOGIN_DISCONNECT        (EQERR_BASE_ACCOUT + 11)
//用户权限不足
#define EQERR_ACCESS_INSUFFICIENCE     (EQERR_BASE_ACCOUT + 12)

#pragma  endregion    

#pragma region 一般性错误             
//获取交易日失败                      
#define EQERR_GET_TRADE_FAIL          (EQERR_BASE_GENERAL + 1)
//初始化主类失败                
#define EQERR_INIT_OBTAIN_CLASS_FAIL  (EQERR_BASE_GENERAL + 2)
//申请内存失败                        
#define EQERR_NEW_MEM_FAIL            (EQERR_BASE_GENERAL + 3)
//解析数据错误                        
#define EQERR_PARSE_DATA_ERR          (EQERR_BASE_GENERAL + 4)
//gzip解压失败                        
#define EQERR_UNGZIP_DATA_FAIL        (EQERR_BASE_GENERAL + 5)
//未知错误
#define EQERR_UNKNOWN_ERR             (EQERR_BASE_GENERAL + 6)
//函数内部错误
#define EQERR_FUNCTION_INTERNAL_ERR   (EQERR_BASE_GENERAL + 7)
//数组越界
#define EQERR_OUTOF_BOUNDS            (EQERR_BASE_GENERAL + 8)
//无数据
#define EQERR_NO_DATA                 (EQERR_BASE_GENERAL + 9)
//系统级别错误
#define EQERR_SYSTEM_ERROR            (EQERR_BASE_GENERAL + 10)
//服务器列表错误                         
#define EQERR_SERVERLIST_ERROR        (EQERR_BASE_GENERAL + 11)
//操作失败
#define EQERR_OPERATION_FAILURE       (EQERR_BASE_GENERAL + 12)
//服务出错
#define EQERR_SERVICE_ERROR           (EQERR_BASE_GENERAL + 13)

#pragma  endregion                    

#pragma region 网络相关错误           
//网络错误                            
#define EQERR_SOCKET_ERR              (EQERR_BASE_NET + 1)
//网络连接失败                        
#define EQERR_CONNECT_FAIL            (EQERR_BASE_NET + 2)
//网络连接超时                        
#define EQERR_CONNECT_TIMEOUT         (EQERR_BASE_NET + 3)
//网络接收时连接断开
#define EQERR_RECVCONNECTION_CLOSED   (EQERR_BASE_NET + 4)
//网络发送失败                        
#define EQERR_SENDSOCK_FAIL           (EQERR_BASE_NET + 5)
//网络发送超时                        
#define EQERR_SENDSOCK_TIMEOUT        (EQERR_BASE_NET + 6)
//网络接收错误                        
#define EQERR_RECVSOCK_FAIL           (EQERR_BASE_NET + 7)
//网络接收超时                        
#define EQERR_RECVSOCK_TIMEOUT        (EQERR_BASE_NET + 8)
#pragma  endregion

#pragma region 参数或请求错误
//传入参数为空
#define EQERR_INPARAM_EMPTY       (EQERR_BASE_PARAM + 1)
//传出参数为空
#define EQERR_OUTPARAM_EMPTY      (EQERR_BASE_PARAM + 2)
//参数错误
#define EQERR_PARAM_ERR           (EQERR_BASE_PARAM + 3)
//起始日期格式不正确
#define EQERR_START_DATE_ERR      (EQERR_BASE_PARAM + 4)
//截止日期格式不正确  
#define EQERR_END_DATE_ERR        (EQERR_BASE_PARAM + 5)
//起始日期大于截至日期
#define EQERR_START_BIGTHAN_END   (EQERR_BASE_PARAM + 6)
//日期格式不正确  
#define EQERR_DATE_ERR            (EQERR_BASE_PARAM + 7)
//无效的证券代码
#define EQERR_CODE_INVALIED       (EQERR_BASE_PARAM + 8)
//证券代码重复
#define EQERR_CODE_REPEAT         (EQERR_BASE_PARAM + 9)
//无效的指标
#define EQERR_INDICATOR_INVALIED  (EQERR_BASE_PARAM + 10)
//用户名为空
#define EQERR_USERNAME_EMPTY      (EQERR_BASE_PARAM + 11)
//密码为空
#define EQERR_PASSWORD_EMPTY      (EQERR_BASE_PARAM + 12)
//订阅数或股票总数达到上限
#define EQERR_TO_UPPER_LIMIT      (EQERR_BASE_PARAM + 13)
//不支持的混合指标
#define EQERR_MIXED_INDICATOR     (EQERR_BASE_PARAM + 14)
//单次订阅指标达到上限
#define EQERR_INDICATOR_TO_UPPER_LIMIT  (EQERR_BASE_PARAM + 15)
//超出日期支持范围
#define EQERR_BEYOND_DATE_SUPPORT (EQERR_BASE_PARAM + 16)
//复权基期小于截止日期
#define EQERR_BASE_LESS_THAN_END  (EQERR_BASE_PARAM + 17)
//不支持的混合证券品种
#define EQERR_MIXED_CODES_MARKET  (EQERR_BASE_PARAM + 18)
//不支持的证券代码品种
#define EQERR_NO_SUPPORT_CODES_MARKET  (EQERR_BASE_PARAM + 19)

#pragma  endregion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//语言类型枚举
typedef enum
{
    eLang_ch,
    eLang_en,
} EQLang;

//EQVARTYPE数据类型枚举
typedef enum 
{
    eVT_null,
    eVT_char,
    eVT_byte,
    eVT_bool,
    eVT_short,
    eVT_ushort,
    eVT_int,
    eVT_uInt,
    eVT_int64,
    eVT_uInt64,
    eVT_float,
    eVT_double,
    eVT_byteArray,
    eVT_asciiString,
    eVT_unicodeString
} EQVarType;

// 返回Events结构类型定义
typedef enum 
{
    eMT_err,                             // 出错信息
    eMT_response,                        // 数据返回
    eMT_partialResponse,                 // 部分数据返回（订阅时适用）
    eMT_others,                          // 其他信息
} EQMsgType;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//char数组封装结构体
typedef struct _tagEQCHAR
{
    char* pChar;                                   //字符数组指针
    unsigned int nSize;                            //字符数组长度，(存储字符串时包括0结尾长度）
} EQCHAR;

//char二维数组封装结构体
typedef struct _tagEQCHARARRAY
{
    EQCHAR* pChArray;                              //EQCHAR数组指针
    unsigned int nSize;                            //EQCHAR数组长度
} EQCHARARRAY;

//EQVARIENT数据类型封装结构体，可存储多种数据类型
typedef struct _tagEQVARIENT
{
    EQVarType vtype;                                //值类型
    union 
    {
        char charValue;
        bool boolValue;
        short shortValue;
        unsigned short uShortValue;
        int  intValue;
        unsigned int uIntValue;
        int64_t  int64Value;
        uint64_t uInt64Value;
        float  floatValue;
        double doubleValue;
    } unionValues;                                   //联合体
    EQCHAR eqchar;                                   //字符数组
} EQVARIENT;

//EQVARIENT的数组封装结构体
typedef struct _tagEQVARIENTARRAY
{
    EQVARIENT* pEQVarient;                           //EQVARIENT数组指针
    unsigned int nSize;                              //EQVARIENT数组长度
} EQVARIENTARRAY;

//EQDATA数据返回结构体
typedef struct _tagEQDATA
{
    EQCHARARRAY codeArray;                           //股票代码数组
    EQCHARARRAY indicatorArray;                      //指标数组
    EQCHARARRAY dateArray;                           //日期数组
    EQVARIENTARRAY valueArray;                       //值数组

    //operator ()(int,int,int)
    EQVARIENT* operator()( const unsigned int code, const unsigned int indicator,const unsigned int date=0 )
    {
        EQVARIENT* pEQVar = NULL;
        if(valueArray.nSize == 0) 
        {
            return pEQVar;
        }

        if(valueArray.nSize != codeArray.nSize * dateArray.nSize * indicatorArray.nSize)
        {
            return pEQVar;
        }

        if(code < codeArray.nSize && indicator < indicatorArray.nSize && date < dateArray.nSize)
        {
            int nIndex = codeArray.nSize * indicatorArray.nSize * date + indicatorArray.nSize * code + indicator;
            pEQVar = valueArray.pEQVarient;
            pEQVar += nIndex;
        }

        return pEQVar;
    }
    //EQVARIENT*
    EQVARIENT* GetAt(const unsigned int pos)
    {
        if(pos >= valueArray.nSize)
        {
            return NULL;
        }
        return &(valueArray.pEQVarient[pos]);
    }

} EQDATA, *PEQDATA;

// 登录信息
#define MAXSTRING 255
typedef struct _tagEQLOGININFO
{
    char userName[MAXSTRING];                        //登录用户名
    char password[MAXSTRING];                        //登录密码
} EQLOGININFO;

//EQMSG数据结构体(异步返回回调时使用)
typedef struct _tagEQMSG
{
    int version;							         //版本号
    EQMsgType msgType;                               //Msg类型
    EQErr err;							             //错误码
    EQID requestID;						             //请求ID
    EQID serialID;						             //流水号
    EQDATA* pEQData;      		                     //包含的数据
} EQMSG, *PEQMSG;


//报表输出结构体
typedef struct _tagEQCtrData
{
	int            row;                              //行数
	int            column;                           //列数
	EQCHARARRAY    indicatorArray;                   //指标名数组
	EQVARIENTARRAY valueArray;                       //值数组

	//operator ()(int,int)
	EQVARIENT* operator()( const unsigned int rowIndex, const unsigned int columnIndex)
	{
		EQVARIENT* pEQVar = NULL;
		if(valueArray.nSize == 0) 
		{
			return pEQVar;
		}

		if(valueArray.nSize != row * column)
		{
			return pEQVar;
		}

		if(rowIndex < row && columnIndex < column)
		{
			int nIndex = column * rowIndex + columnIndex;
			pEQVar = valueArray.pEQVarient;
			pEQVar += nIndex;
		}

		return pEQVar;
	}
	//EQVARIENT*
	EQVARIENT* GetAt(const unsigned int pos)
	{
		if(pos >= valueArray.nSize)
		{
			return NULL;
		}
		return &(valueArray.pEQVarient[pos]);
	}

} EQCTRDATA;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//日志回调函数
typedef int (*logcallback)(const char* log);

//请求回调函数(异步请求时使用)
typedef int (*datacallback) (const EQMSG* pMsg, LPVOID lpUserParam);

//用户可以自定义ServerList.json文件的存放路径
EMQUANTAPI void setserverlistdir(const char* dir);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//初始化和登陆(开始时调用)  options：附加参数 "TestLatency=1"
EMQUANTAPI EQErr start(EQLOGININFO* pLoginInfo, const char* options, logcallback pfnCallback);
//退出(结束时调用)
EMQUANTAPI EQErr stop();
//设置主回调函数，在start后调用
EMQUANTAPI EQErr setcallback(datacallback pfnCallback);
//获取错误码文本说明
EMQUANTAPI const char* geterrstring(EQErr errcode, EQLang lang=eLang_en);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//参数说明:
//    codes: 东财代码  多个代码间用半角逗号隔开，支持大小写。如 "300059.SZ,000002.SZ,000003.SZ,000004.SZ"
//    indicators: 东财指标 多个指标间用半角逗号隔开，支持大小写。如 "open,close,high"
//    startdate和enddate: 开始日期和截止日期。如无分隔符，则必须为8位数字。格式支持:YYYYMMDD YYYY/MM/DD YYYY/M/D YYYY-MM-DD YYYY-M-D
//    options: 附加参数  多个参数以半角逗号隔开，"Period=1,Market=CNSESH,Order=1,Adjustflag=1,Curtype=1,Pricetype=1,Type=1"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//指标服务数据查询(同步请求)
EMQUANTAPI EQErr csd(const char* codes, const char* indicators, const char* startDate, const char* endDate, const char* options, EQDATA*& pEQData);
//截面数据查询(同步请求)
EMQUANTAPI EQErr css(const char* codes, const char* indicators, const char* options, EQDATA*& pEQData);
//获取区间日期内的交易日(同步请求)
EMQUANTAPI EQErr tradedates(const char* startDate, const char* endDate, const char* options, EQDATA*& pEQData);
//获取系统板块成分(同步请求)
EMQUANTAPI EQErr sector(const char* pukeyCode, const char* tradeDate, const char* options, EQDATA*& pEQData);
//获取偏移N的交易日(同步请求)
#ifdef WIN32 
EMQUANTAPI EQErr getdate(const char* tradeDate, int offDay, const char* options, EQDATA*& pEQData);
#else
EMQUANTAPI EQErr gettradedate(const char* tradeDate, int offDay, const char* options, EQDATA*& pEQData);
#endif
//历史分钟K线(同步请求) //code只支持单个股票
EMQUANTAPI EQErr csc(const char* code, const char* indicators, const char* startDate, const char* endDate, const char* options, EQDATA*& pEQData);
//行情快照(同步请求) 每次indicators最多为64个
EMQUANTAPI EQErr csqsnapshot(const char* codes, const char* indicators, const char* options, EQDATA*& pEQData);
//获取专题报表(同步请求)
EMQUANTAPI EQErr ctr(const char* ctrName, const char* indicators, const char* options, EQCTRDATA*& pEQCtrData);
//仅供静态数据同步接口返回数据指针释放内存
EMQUANTAPI EQErr releasedata(void* pEQData);


//以下为异步函数
//返回： EQID  流水号
//pfnCallback：不同的接口可以设定不同的回调，传NULL则使用默认的主回调函数
//lpUserParam: 用户参数,回调时原样返回

//实时行情(异步)  每次indicators最多为64个 options: Pushtype=0 增量推送  1全量推送
EMQUANTAPI EQID csq(const char* codes, const char* indicators, const char* options, datacallback pfnCallback, LPVOID lpUserParam);
//取消实时行情订阅   serialID为0时 取消所有订阅
EMQUANTAPI EQErr csqcancel(EQID serialID);

//日内跳价服务(异步)  startdatetime和enddatetime格式(YYYYMMDDHHMMSS或HHMMSS表示系统日期当天的时间，两者需使用同一种格式)
EMQUANTAPI EQID cst(const char* codes, const char* indicators, const char* startdatetime, const char* enddatetime, const char* options, datacallback pfnCallback, LPVOID lpUserParam);

#endif /*#ifndef _EMQUANTAPI_H_*/
