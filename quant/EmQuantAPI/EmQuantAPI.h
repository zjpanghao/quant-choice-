/**************************************************************************************************
*   EmQuantAPI.h    version  2.0.1.0                                                              *                                                                      *
*   Copyright(c)2016-2017,  EastMoney Information  Co,. Ltd. All Rights Reserved.                 *
*   Eastmoney API must not be used without authorzation, unauthorized user shall be held liable.  *                                                                  *
***************************************************************************************************/
#ifndef _EMQUANTAPI_H_
#define _EMQUANTAPI_H_ 

#include <stdint.h>

#define EMQUNANTAPIVER 2200

#ifdef WIN32
#ifdef EMQUANTAPI_EXPORTS
#define EMQUANTAPI  extern "C" __declspec(dllexport)
#else
#define EMQUANTAPI extern "C" __declspec(dllimport)
#endif /*#ifdef EMQUANTAPI_EXPORTS*/
#else
#define EMQUANTAPI extern "C"
#endif /*#ifdef WIN32*/

//´íÎóÂëµÄÀàĞÍ¶¨Òå
typedef int EQErr;

//ÇëÇóIDµÄÀàĞÍ¶¨Òå
typedef int EQID;

#ifndef WIN32
typedef void* LPVOID;
#endif //#ifndef WIN32
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//³É¹¦
#define EQERR_SUCCESS                  (0)
//´íÎó»ùÊı                             
#define EQERR_BASE                     (10000000)
//Ò»°ãĞÔ´íÎó                           
#define EQERR_BASE_GENERAL             (EQERR_BASE)
//ÕË»§Ïà¹Ø´íÎó						 
#define EQERR_BASE_ACCOUT              (EQERR_BASE + 1000)
//ÍøÂçÏà¹Ø´íÎó                         
#define EQERR_BASE_NET                 (EQERR_BASE + 2000)
//²ÎÊı»òÇëÇó´íÎó                       
#define EQERR_BASE_PARAM               (EQERR_BASE + 3000)


#pragma region ÕË»§Ïà¹Ø´íÎó 
//ÓÃ»§Î´µÇÂ¼
#define EQERR_NO_LOGIN                 (EQERR_BASE_ACCOUT + 1)
//ÓÃ»§Ãû»òÃÜÂë´íÎó					   
#define EQERR_USERNAMEORPASSWORD_ERR   (EQERR_BASE_ACCOUT + 2)
//ÓÃ»§ÎŞAPIÈ¨ÏŞ						  
#define EQERR_NO_ACCESS                (EQERR_BASE_ACCOUT + 3)
//ÓÃ»§APIÈ¨ÏŞ¹ıÆÚ						 
#define EQERR_ACCESS_EXPIRE            (EQERR_BASE_ACCOUT + 4)
//»ñÈ¡ÓÃ»§ĞÅÏ¢Ê§°Ü					   
#define EQERR_GETUSERINFO_FAIL         (EQERR_BASE_ACCOUT + 5)
//DLL°æ±¾ºÅ¹ıÆÚ						  
#define EQERR_DLLVESION_EXPIRE         (EQERR_BASE_ACCOUT + 6)
//ÓÃ»§ÎŞAPI_LV2È¨ÏŞ					  
#define EQERR_NO_LV2_ACCESS            (EQERR_BASE_ACCOUT + 7)
//ÓÃ»§API_LV2È¨ÏŞ¹ıÆÚ					 
#define EQERR_LV2_ACCESS_EXPIRE        (EQERR_BASE_ACCOUT + 8)
//ÕËºÅµÇÂ¼Êı´ïµ½ÉÏÏŞ					  
#define EQERR_LOGIN_COUNT_LIMIT        (EQERR_BASE_ACCOUT + 9)
//ÓÃ»§µÇÂ¼Ê§°Ü						 
#define EQERR_LOGIN_FAIL               (EQERR_BASE_ACCOUT + 10)
//ÓÃ»§µÇÂ¼µôÏß						 
#define EQERR_LOGIN_DISCONNECT         (EQERR_BASE_ACCOUT + 11)
//ÓÃ»§È¨ÏŞ²»×ã						 
#define EQERR_ACCESS_INSUFFICIENCE     (EQERR_BASE_ACCOUT + 12)
//ÓÃ»§ÕıÔÚµÇÂ¼						 
#define EQERR_IS_LOGIN                 (EQERR_BASE_ACCOUT + 13)
//ĞèÒªµÇÂ¼¼¤»î						 
#define EQERR_NEED_ACTIVATE            (EQERR_BASE_ACCOUT + 14)
//µÇÂ¼·şÎñÒì³£						 
#define EQERR_LOGIN_SERVICE_ERR        (EQERR_BASE_ACCOUT + 15)
//ÕıÔÚÈË¹¤¼¤»î						 
#define EQERR_IS_MANUAL_ACTIVATE       (EQERR_BASE_ACCOUT + 16)
//ÎŞĞèÈË¹¤¼¤»î						 
#define EQERR_NOTNEED_MANUAL_ACTIVATE  (EQERR_BASE_ACCOUT + 17)
//ÈË¹¤¼¤»îÊ§°Ü						 
#define EQERR_MANUAL_ACTIVATE_FAIL     (EQERR_BASE_ACCOUT + 18)
//¼¤»îÉè±¸ÓëµÇÂ¼Éè±¸²»Ò»ÖÂ			   
#define EQERR_DIFFRENT_DEVICE          (EQERR_BASE_ACCOUT + 19)
//userInfoÒÑÊ§Ğ§ĞèÖØĞÂ¼¤»î			   
#define EQERR_USERINFO_EXPIRED         (EQERR_BASE_ACCOUT + 20)
#pragma  endregion    

#pragma region Ò»°ãĞÔ´íÎó             
//»ñÈ¡½»Ò×ÈÕÊ§°Ü                      
#define EQERR_GET_TRADE_FAIL           (EQERR_BASE_GENERAL + 1)
//³õÊ¼»¯Ö÷ÀàÊ§°Ü                	    
#define EQERR_INIT_OBTAIN_CLASS_FAIL   (EQERR_BASE_GENERAL + 2)
//ÉêÇëÄÚ´æÊ§°Ü                         
#define EQERR_NEW_MEM_FAIL             (EQERR_BASE_GENERAL + 3)
//½âÎöÊı¾İ´íÎó                         
#define EQERR_PARSE_DATA_ERR           (EQERR_BASE_GENERAL + 4)
//gzip½âÑ¹Ê§°Ü                         
#define EQERR_UNGZIP_DATA_FAIL         (EQERR_BASE_GENERAL + 5)
//Î´Öª´íÎó							   
#define EQERR_UNKNOWN_ERR              (EQERR_BASE_GENERAL + 6)
//º¯ÊıÄÚ²¿´íÎó						 
#define EQERR_FUNCTION_INTERNAL_ERR    (EQERR_BASE_GENERAL + 7)
//Êı×éÔ½½ç							   
#define EQERR_OUTOF_BOUNDS             (EQERR_BASE_GENERAL + 8)
//ÎŞÊı¾İ							    
#define EQERR_NO_DATA                  (EQERR_BASE_GENERAL + 9)
//ÏµÍ³¼¶±ğ´íÎó						 
#define EQERR_SYSTEM_ERROR             (EQERR_BASE_GENERAL + 10)
//·şÎñÆ÷ÁĞ±í´íÎó                          
#define EQERR_SERVERLIST_ERROR         (EQERR_BASE_GENERAL + 11)
//²Ù×÷Ê§°Ü							   
#define EQERR_OPERATION_FAILURE        (EQERR_BASE_GENERAL + 12)
//·şÎñ³ö´í							   
#define EQERR_SERVICE_ERROR            (EQERR_BASE_GENERAL + 13)
//¿¿¿¿¿¿¿¿¿
#define EQERR_GETSERVERLIST_FAIL       (EQERR_BASE_GENERAL + 14)

#pragma  endregion                    

#pragma region ÍøÂçÏà¹Ø´íÎó           
//ÍøÂç´íÎó                            
#define EQERR_SOCKET_ERR               (EQERR_BASE_NET + 1)
//ÍøÂçÁ¬½ÓÊ§°Ü                         
#define EQERR_CONNECT_FAIL             (EQERR_BASE_NET + 2)
//ÍøÂçÁ¬½Ó³¬Ê±                         
#define EQERR_CONNECT_TIMEOUT          (EQERR_BASE_NET + 3)
//ÍøÂç½ÓÊÕÊ±Á¬½Ó¶Ï¿ª					  
#define EQERR_RECVCONNECTION_CLOSED    (EQERR_BASE_NET + 4)
//ÍøÂç·¢ËÍÊ§°Ü                         
#define EQERR_SENDSOCK_FAIL            (EQERR_BASE_NET + 5)
//ÍøÂç·¢ËÍ³¬Ê±                         
#define EQERR_SENDSOCK_TIMEOUT         (EQERR_BASE_NET + 6)
//ÍøÂç½ÓÊÕ´íÎó                         
#define EQERR_RECVSOCK_FAIL            (EQERR_BASE_NET + 7)
//ÍøÂç½ÓÊÕ³¬Ê±                         
#define EQERR_RECVSOCK_TIMEOUT         (EQERR_BASE_NET + 8)
#pragma  endregion

#pragma region ²ÎÊı»òÇëÇó´íÎó
//´«Èë²ÎÊıÎª¿Õ
#define EQERR_INPARAM_EMPTY            (EQERR_BASE_PARAM + 1)
//´«³ö²ÎÊıÎª¿Õ					       
#define EQERR_OUTPARAM_EMPTY           (EQERR_BASE_PARAM + 2)
//²ÎÊı´íÎó						       
#define EQERR_PARAM_ERR                (EQERR_BASE_PARAM + 3)
//ÆğÊ¼ÈÕÆÚ¸ñÊ½²»ÕıÈ·				      
#define EQERR_START_DATE_ERR           (EQERR_BASE_PARAM + 4)
//½ØÖ¹ÈÕÆÚ¸ñÊ½²»ÕıÈ·  			       
#define EQERR_END_DATE_ERR             (EQERR_BASE_PARAM + 5)
//ÆğÊ¼ÈÕÆÚ´óÓÚ½ØÖÁÈÕÆÚ			       
#define EQERR_START_BIGTHAN_END        (EQERR_BASE_PARAM + 6)
//ÈÕÆÚ¸ñÊ½²»ÕıÈ·  				        
#define EQERR_DATE_ERR                 (EQERR_BASE_PARAM + 7)
//ÎŞĞ§µÄÖ¤È¯´úÂë				        
#define EQERR_CODE_INVALIED            (EQERR_BASE_PARAM + 8)
//Ö¤È¯´úÂëÖØ¸´					       
#define EQERR_CODE_REPEAT              (EQERR_BASE_PARAM + 9)
//ÎŞĞ§µÄÖ¸±ê						      
#define EQERR_INDICATOR_INVALIED       (EQERR_BASE_PARAM + 10)
//ÓÃ»§ÃûÎª¿Õ						      
#define EQERR_USERNAME_EMPTY           (EQERR_BASE_PARAM + 11)
//ÃÜÂëÎª¿Õ						       
#define EQERR_PASSWORD_EMPTY           (EQERR_BASE_PARAM + 12)
//¶©ÔÄÊı»ò¹ÉÆ±×ÜÊı´ïµ½ÉÏÏŞ		       
#define EQERR_TO_UPPER_LIMIT           (EQERR_BASE_PARAM + 13)
//²»Ö§³ÖµÄ»ìºÏÖ¸±ê				       
#define EQERR_MIXED_INDICATOR          (EQERR_BASE_PARAM + 14)
//µ¥´Î¶©ÔÄÖ¸±ê´ïµ½ÉÏÏŞ				 
#define EQERR_INDICATOR_TO_UPPER_LIMIT (EQERR_BASE_PARAM + 15)
//³¬³öÈÕÆÚÖ§³Ö·¶Î§					   
#define EQERR_BEYOND_DATE_SUPPORT      (EQERR_BASE_PARAM + 16)
//¸´È¨»ùÆÚĞ¡ÓÚ½ØÖ¹ÈÕÆÚ			       
#define EQERR_BASE_LESS_THAN_END       (EQERR_BASE_PARAM + 17)
//²»Ö§³ÖµÄ»ìºÏÖ¤È¯Æ·ÖÖ			       
#define EQERR_MIXED_CODES_MARKET       (EQERR_BASE_PARAM + 18)
//²»Ö§³ÖµÄÖ¤È¯´úÂëÆ·ÖÖ				 
#define EQERR_NO_SUPPORT_CODES_MARKET  (EQERR_BASE_PARAM + 19)
//½»Ò×ÌõÊı³¬¹ıÉÏÏŞ					   
#define EQERR_ORDER_TO_UPPER_LIMIT     (EQERR_BASE_PARAM + 20)
//²»Ö§³ÖµÄ½»Ò×ĞÅÏ¢				       
#define EQERR_NO_SUPPORT_ORDERINFO     (EQERR_BASE_PARAM + 21)
//Ö¸±êÖØ¸´						 
#define EQERR_INDICATOR_REPEAT         (EQERR_BASE_PARAM + 22)
#pragma  endregion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//´íÎóĞÅÏ¢ÓïÑÔÀàĞÍÃ¶¾Ù
typedef enum
{
    eLang_ch,
    eLang_en,
} EQLang;

//EQVARTYPEÊı¾İÀàĞÍÃ¶¾Ù
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

// ·µ»ØEvents½á¹¹ÀàĞÍÃ¶¾Ù
typedef enum 
{
    eMT_err,                             // ³ö´íĞÅÏ¢
    eMT_response,                        // Êı¾İ·µ»Ø
    eMT_partialResponse,                 // ²¿·ÖÊı¾İ·µ»Ø£¨¶©ÔÄÊ±ÊÊÓÃ£©
    eMT_others,                          // ÆäËûĞÅÏ¢
} EQMsgType;

//ÍøÂç´úÀí·½Ê½Ã¶¾Ù
typedef enum 
{
	ePT_NONE,                            // ²»Ê¹ÓÃ´úÀí  
	ePT_HTTP,						     // HTTP´úÀí
	ePT_HTTPS,						     // HTTPS´úÀí
	ePT_SOCK4,						     // SOCK4´úÀí
	ePT_SOCK5                            // SOCK5´úÀí
} ProxyType;

//ÏÂµ¥²Ù×÷ÀàĞÍÃ¶¾Ù
typedef enum 
{
	eOT_default,                         // Ä¬ÈÏ(Ä¬ÈÏÔò¸ù¾İ´«ÈëÊıÁ¿µÄÕı¸º±êÖ¾ÂòÈëeOT_buyÂô³öeOT_Sell,ÆäÓàÀàĞÍ¶ÔÊıÁ¿×÷Õı¸º×ª»»)
	eOT_buy,						     // ÂòÈë
	eOT_sell,						     // Âô³ö
	eOT_purchase,						 // Éê¹º
	eOT_redemption,                      // Êê»Ø
} OperateType;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push)
#pragma pack(8)

//charÊı×é·â×°½á¹¹Ìå
typedef struct _tagEQCHAR
{
    char* pChar;                                   //×Ö·ûÊı×éÖ¸Õë
    unsigned int nSize;                            //×Ö·ûÊı×é³¤¶È£¬(´æ´¢×Ö·û´®Ê±°üÀ¨0½áÎ²³¤¶È£©
} EQCHAR;

//char¶şÎ¬Êı×é·â×°½á¹¹Ìå
typedef struct _tagEQCHARARRAY
{
    EQCHAR* pChArray;                              //EQCHARÊı×éÖ¸Õë
    unsigned int nSize;                            //EQCHARÊı×é³¤¶È
} EQCHARARRAY;

//EQVARIENTÊı¾İÀàĞÍ·â×°½á¹¹Ìå£¬¿É´æ´¢¶àÖÖÊı¾İÀàĞÍ
typedef struct _tagEQVARIENT
{
    EQVarType vtype;                                //ÖµÀàĞÍ
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
    } unionValues;                                   //ÁªºÏÌå
    EQCHAR eqchar;                                   //×Ö·ûÊı×é
} EQVARIENT;

//EQVARIENTµÄÊı×é·â×°½á¹¹Ìå
typedef struct _tagEQVARIENTARRAY
{
    EQVARIENT* pEQVarient;                           //EQVARIENTÊı×éÖ¸Õë
    unsigned int nSize;                              //EQVARIENTÊı×é³¤¶È
} EQVARIENTARRAY;

//EQDATAÊı¾İ·µ»Ø½á¹¹Ìå
typedef struct _tagEQDATA
{
    EQCHARARRAY codeArray;                           //¹ÉÆ±´úÂëÊı×é
    EQCHARARRAY indicatorArray;                      //Ö¸±êÊı×é
    EQCHARARRAY dateArray;                           //ÈÕÆÚÊı×é
    EQVARIENTARRAY valueArray;                       //ÖµÊı×é

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

//EQMSGÊı¾İ½á¹¹Ìå(Òì²½·µ»Ø»Øµ÷Ê±Ê¹ÓÃ)
typedef struct _tagEQMSG
{
    int version;							         //°æ±¾ºÅ
    EQMsgType msgType;                               //MsgÀàĞÍ
    EQErr err;							             //´íÎóÂë
    EQID requestID;						             //ÇëÇóID
    EQID serialID;						             //Á÷Ë®ºÅ
    EQDATA* pEQData;      		                     //°üº¬µÄÊı¾İ
} EQMSG, *PEQMSG;

//±¨±íÊä³ö½á¹¹Ìå
typedef struct _tagEQCtrData
{
	int            row;                              //ĞĞÊı
	int            column;                           //ÁĞÊı
	EQCHARARRAY    indicatorArray;                   //Ö¸±êÃûÊı×é
	EQVARIENTARRAY valueArray;                       //ÖµÊı×é

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

// µÇÂ¼ĞÅÏ¢
#define MAXSTRING 255
typedef struct _tagEQLOGININFO
{
	char userName[MAXSTRING];                        //µÇÂ¼ÓÃ»§Ãû
	char password[MAXSTRING];                        //µÇÂ¼ÃÜÂë
} EQLOGININFO;

//ÏÂµ¥ĞÅÏ¢
typedef struct _tagORDERINFO
{
	char         code[16];                           //Ö¤È¯´úÂë
	double       volume;                             //½»Ò×ÊıÁ¿
	float        price;                              //½»Ò×¼Û¸ñ
	int          date;                               //½»Ò×ÈÕÆÚ yyyymmdd  8Î»Êı
	int          time;                               //½»Ò×Ê±¼ä hhmmss    6Î»Êı
	OperateType  optype;                             //²Ù×÷ÀàĞÍ
	float        cost;                               //·ÑÓÃ
	float        rate;                               //·ÑÂÊ
	int          reserve;                            //±£Áô×Ö¶Î
} ORDERINFO, *PORDERINFO;

#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ÈÕÖ¾»Øµ÷º¯Êı
typedef int (*logcallback)(const char* log);

//ÇëÇó»Øµ÷º¯Êı(Òì²½ÇëÇóÊ±Ê¹ÓÃ)
typedef int (*datacallback) (const EQMSG* pMsg, LPVOID lpUserParam);

//ÉèÖÃÖ÷»Øµ÷º¯Êı(Ò»¶¨ÒªÉèÖÃÒ»¸öÖ÷»Øµ÷º¯Êı£¬·ñÔòÊÕ²»µ½ÕËºÅµôÏßÍ¨Öª)
EMQUANTAPI EQErr setcallback(datacallback pfnCallback);

//ÓÃ»§¿ÉÒÔ×Ô¶¨ÒåServerList.jsonÎÄ¼şµÄ´æ·ÅÂ·¾¶
EMQUANTAPI void setserverlistdir(const char* dir);

//»ñÈ¡´íÎóÂëÎÄ±¾ËµÃ÷
EMQUANTAPI const char* geterrstring(EQErr errcode, EQLang lang=eLang_en);

//ÉèÖÃÍøÂç´úÀí ×¢£ºÈçĞèÊ¹ÓÃ´úÀí£¬ĞèÒªÔÚµ÷ÓÃËùÓĞ½Ó¿ÚÖ®Ç°ÉèÖÃ
EMQUANTAPI EQErr setproxy(ProxyType type, const char* proxyip, unsigned short proxyport, bool verify, const char* proxyuser, const char* proxypwd);

/**ÈË¹¤¼¤»î 
*  pLoginInfo£ºÕË»§ÃûÃÜÂë½á¹¹ÌåÖ¸Õë£¨±Ø´«Ïî£© options: ¿É´« "email=who@what.com"  pfnCallbackÈÕÖ¾»Øµ÷º¯Êı
*  ×¢£ºÈË¹¤¼¤»îÊÊÓÃÓÚÎŞ½çÃæÔËĞĞ»·¾³£¨ÈçÔ¶³Ìlinux£©»òÎŞ·¨ÔËĞĞLoginActivator³ÌĞòµÄÇé¿ö£¬
*      ¼¤»î³É¹¦ºó½«»ñµÃµÄ¼¤»îÎÄ¼ş"userInfo"·Åµ½"ServerList.json"Í¬¼¶Ä¿Â¼£¬ÔÙµ÷ÓÃstartµÇÂ¼*/  
EMQUANTAPI EQErr manualactivate(EQLOGININFO* pLoginInfo, const char* options, logcallback pfnCallback);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//³õÊ¼»¯ºÍµÇÂ¼(¿ªÊ¼Ê±»òµôÏßºóµ÷ÓÃ)  pLoginInfo£º±£Áô²ÎÊı,ÎŞĞè´«(2.0.0.0°æ±¾Ö®ºó¸ÄÎªÁîÅÆ×Ô¶¯µÇÂ½)  options£º¸½¼Ó²ÎÊı  pfnCallbackÈÕÖ¾»Øµ÷º¯Êı
EMQUANTAPI EQErr start(EQLOGININFO* pLoginInfo, const char* options, logcallback pfnCallback);
//ÍË³ö(½áÊøÍË³öÊ±µ÷ÓÃ£¬Ö»Ğèµ÷ÓÃÒ»´Î)
EMQUANTAPI EQErr stop();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//²ÎÊıËµÃ÷:
//    codes: ¶«²Æ´úÂë  ¶à¸ö´úÂë¼äÓÃ°ë½Ç¶ººÅ¸ô¿ª£¬Ö§³Ö´óĞ¡Ğ´¡£Èç "300059.SZ,000002.SZ,000003.SZ,000004.SZ"
//    indicators: ¶«²ÆÖ¸±ê ¶à¸öÖ¸±ê¼äÓÃ°ë½Ç¶ººÅ¸ô¿ª£¬Ö§³Ö´óĞ¡Ğ´¡£Èç "open,close,high"
//    startdateºÍenddate: ¿ªÊ¼ÈÕÆÚºÍ½ØÖ¹ÈÕÆÚ¡£ÈçÎŞ·Ö¸ô·û£¬Ôò±ØĞëÎª8Î»Êı×Ö¡£¸ñÊ½Ö§³Ö:YYYYMMDD YYYY/MM/DD YYYY/M/D YYYY-MM-DD YYYY-M-D
//    options: ¸½¼Ó²ÎÊı  ¶à¸ö²ÎÊıÒÔ°ë½Ç¶ººÅ¸ô¿ª£¬"Period=1,Market=CNSESH,Order=1,Adjustflag=1,Curtype=1,Pricetype=1,Type=1"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Ö¸±ê·şÎñÊı¾İ²éÑ¯(Í¬²½ÇëÇó)
EMQUANTAPI EQErr csd(const char* codes, const char* indicators, const char* startDate, const char* endDate, const char* options, EQDATA*& pEQData);
//½ØÃæÊı¾İ²éÑ¯(Í¬²½ÇëÇó)
EMQUANTAPI EQErr css(const char* codes, const char* indicators, const char* options, EQDATA*& pEQData);
//»ñÈ¡Çø¼äÈÕÆÚÄÚµÄ½»Ò×ÈÕ(Í¬²½ÇëÇó)
EMQUANTAPI EQErr tradedates(const char* startDate, const char* endDate, const char* options, EQDATA*& pEQData);
//»ñÈ¡ÏµÍ³°å¿é³É·Ö(Í¬²½ÇëÇó)
EMQUANTAPI EQErr sector(const char* pukeyCode, const char* tradeDate, const char* options, EQDATA*& pEQData);
//»ñÈ¡Æ«ÒÆNµÄ½»Ò×ÈÕ(Í¬²½ÇëÇó)
#ifdef WIN32 
EMQUANTAPI EQErr getdate(const char* tradeDate, int offDay, const char* options, EQDATA*& pEQData);
#else
EMQUANTAPI EQErr gettradedate(const char* tradeDate, int offDay, const char* options, EQDATA*& pEQData);
#endif
//ÀúÊ··ÖÖÓKÏß(Í¬²½ÇëÇó) //codeÖ»Ö§³Öµ¥¸ö¹ÉÆ±
EMQUANTAPI EQErr csc(const char* code, const char* indicators, const char* startDate, const char* endDate, const char* options, EQDATA*& pEQData);
//ĞĞÇé¿ìÕÕ(Í¬²½ÇëÇó) Ã¿´Îindicators×î¶àÎª64¸ö
EMQUANTAPI EQErr csqsnapshot(const char* codes, const char* indicators, const char* options, EQDATA*& pEQData);
//»ñÈ¡×¨Ìâ±¨±í(Í¬²½ÇëÇó)
EMQUANTAPI EQErr ctr(const char* ctrName, const char* indicators, const char* options, EQCTRDATA*& pEQCtrData);

//Ìõ¼şÑ¡¹É(Í¬²½ÇëÇó)
//cps²ÎÊıËµÃ÷£º
//cpsCodes:      Ö¤È¯´úÂë·¶Î§,±Ø´«,Ö§³ÖÁ½ÖÖÄ£Ê½ 1.°å¿é´úÂë ÒÔ B_ ¿ªÍ·£¬Èç "B_001004"; 2. ¶«²Æ´úÂë£¬¶à¸ö´úÂë¼äÓÃ°ë½Ç¶ººÅ¸ô¿ª£¬Èç"000001.SZ,000002.SZ,600000.SH"
//cpsIndicators: ±í´ïÊ½²ÎÊı,±í´ïÊ½Ö®Ç°ÓÃÓ¢ÎÄ·ÖºÅ¸ô¿ª,ÄÚ²¿¸÷²ÎÊıÓÃ°ë½Ç¶ººÅ¸ô¿ª Èç   s1,open,2016/12/13,1;s2,close,2017-02-25,1;s3,listdate
//cpsConditions: Ìõ¼ş±í´ïÊ½
//cpsOptions:    ÆäËû¸½¼ÓÌõ¼ş,ÈçÅÅĞò,È¡Ç°NÌõÑ¡¹É½á¹ûµÈ
//pEQData:       Ñ¡¹É½á¹û
EMQUANTAPI EQErr cps(const char* cpsCodes, const char* cpsIndicators, const char* cpsConditions, const char* cpsOptions, EQDATA*& pEQData);

//×éºÏÕË»§¹ØÏµ²éÑ¯(Í¬²½ÇëÇó)
EMQUANTAPI EQErr pquery(const char* options, EQDATA*& pEQData);

//ÅúÁ¿ÏÂµ¥(Í¬²½ÇëÇó)
EMQUANTAPI EQErr porder(ORDERINFO* pOrderInfo, int orderInfoSize, const char* accountId, const char* remark, const char* options);

//½ö¹©¾²Ì¬Êı¾İÍ¬²½½Ó¿Ú·µ»ØÊı¾İÖ¸ÕëÊÍ·ÅÄÚ´æ
EMQUANTAPI EQErr releasedata(void* pEQData);

//¡ü¡ü¡ü¡ü¡ü¡ü¡üÒÔÉÏÎªÍ¬²½º¯Êı¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü
//¡ı¡ı¡ı¡ı¡ı¡ı¡ıÒÔÏÂÎªÒì²½º¯Êı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı¡ı
//·µ»Ø£º EQID  Á÷Ë®ºÅ
//pfnCallback£º²»Í¬µÄ½Ó¿Ú¿ÉÒÔÉè¶¨²»Í¬µÄ»Øµ÷£¬´«NULLÔòÊ¹ÓÃÄ¬ÈÏµÄÖ÷»Øµ÷º¯Êı
//lpUserParam: ÓÃ»§²ÎÊı,»Øµ÷Ê±Ô­Ñù·µ»Ø

//ÊµÊ±ĞĞÇé(Òì²½)  Ã¿´Îindicators×î¶àÎª64¸ö options: Pushtype=0 ÔöÁ¿ÍÆËÍ  1È«Á¿ÍÆËÍ
EMQUANTAPI EQID csq(const char* codes, const char* indicators, const char* options, datacallback pfnCallback, LPVOID lpUserParam);
//È¡ÏûÊµÊ±ĞĞÇé¶©ÔÄ   serialIDÎª0Ê± È¡ÏûËùÓĞ¶©ÔÄ
EMQUANTAPI EQErr csqcancel(EQID serialID);

//ÈÕÄÚÌø¼Û·şÎñ(Òì²½)  startdatetimeºÍenddatetime¸ñÊ½(YYYYMMDDHHMMSS»òHHMMSS±íÊ¾ÏµÍ³ÈÕÆÚµ±ÌìµÄÊ±¼ä£¬Á½ÕßĞèÊ¹ÓÃÍ¬Ò»ÖÖ¸ñÊ½)
EMQUANTAPI EQID cst(const char* codes, const char* indicators, const char* startdatetime, const char* enddatetime, const char* options, datacallback pfnCallback, LPVOID lpUserParam);

#endif /*#ifndef _EMQUANTAPI_H_*/
