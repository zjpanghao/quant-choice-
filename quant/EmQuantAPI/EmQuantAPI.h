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

//����������Ͷ���
typedef int EQErr;

//����ID�����Ͷ���
typedef int EQID;

#ifndef WIN32
typedef void* LPVOID;
#endif //#ifndef WIN32
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�ɹ�
#define EQERR_SUCCESS                 (0)
//�������                            
#define EQERR_BASE                    (10000000)
//һ���Դ���                          
#define EQERR_BASE_GENERAL            (EQERR_BASE)
//�˻���ش���
#define EQERR_BASE_ACCOUT             (EQERR_BASE + 1000)
//������ش���                        
#define EQERR_BASE_NET                (EQERR_BASE + 2000)
//�������������                      
#define EQERR_BASE_PARAM              (EQERR_BASE + 3000)


#pragma region �˻���ش��� 
//�û�δ��½
#define EQERR_NO_LOGIN                (EQERR_BASE_ACCOUT + 1)
//�û������������
#define EQERR_USERNAMEORPASSWORD_ERR  (EQERR_BASE_ACCOUT + 2)
//�û���APIȨ��
#define EQERR_NO_ACCESS               (EQERR_BASE_ACCOUT + 3)
//�û�APIȨ�޹���
#define EQERR_ACCESS_EXPIRE           (EQERR_BASE_ACCOUT + 4)
//��ȡ�û���Ϣʧ��
#define EQERR_GETUSERINFO_FAIL        (EQERR_BASE_ACCOUT + 5)
//DLL�汾�Ź���
#define EQERR_DLLVESION_EXPIRE        (EQERR_BASE_ACCOUT + 6)
//�û���API_LV2Ȩ��
#define EQERR_NO_LV2_ACCESS           (EQERR_BASE_ACCOUT + 7)
//�û�API_LV2Ȩ�޹���
#define EQERR_LV2_ACCESS_EXPIRE       (EQERR_BASE_ACCOUT + 8)
//�˺ŵ�½���ﵽ����
#define EQERR_LOGIN_COUNT_LIMIT       (EQERR_BASE_ACCOUT + 9)
//�û���½ʧ��
#define EQERR_LOGIN_FAIL              (EQERR_BASE_ACCOUT + 10)
//�û���½����
#define EQERR_LOGIN_DISCONNECT        (EQERR_BASE_ACCOUT + 11)
//�û�Ȩ�޲���
#define EQERR_ACCESS_INSUFFICIENCE     (EQERR_BASE_ACCOUT + 12)

#pragma  endregion    

#pragma region һ���Դ���             
//��ȡ������ʧ��                      
#define EQERR_GET_TRADE_FAIL          (EQERR_BASE_GENERAL + 1)
//��ʼ������ʧ��                
#define EQERR_INIT_OBTAIN_CLASS_FAIL  (EQERR_BASE_GENERAL + 2)
//�����ڴ�ʧ��                        
#define EQERR_NEW_MEM_FAIL            (EQERR_BASE_GENERAL + 3)
//�������ݴ���                        
#define EQERR_PARSE_DATA_ERR          (EQERR_BASE_GENERAL + 4)
//gzip��ѹʧ��                        
#define EQERR_UNGZIP_DATA_FAIL        (EQERR_BASE_GENERAL + 5)
//δ֪����
#define EQERR_UNKNOWN_ERR             (EQERR_BASE_GENERAL + 6)
//�����ڲ�����
#define EQERR_FUNCTION_INTERNAL_ERR   (EQERR_BASE_GENERAL + 7)
//����Խ��
#define EQERR_OUTOF_BOUNDS            (EQERR_BASE_GENERAL + 8)
//������
#define EQERR_NO_DATA                 (EQERR_BASE_GENERAL + 9)
//ϵͳ�������
#define EQERR_SYSTEM_ERROR            (EQERR_BASE_GENERAL + 10)
//�������б����                         
#define EQERR_SERVERLIST_ERROR        (EQERR_BASE_GENERAL + 11)
//����ʧ��
#define EQERR_OPERATION_FAILURE       (EQERR_BASE_GENERAL + 12)
//�������
#define EQERR_SERVICE_ERROR           (EQERR_BASE_GENERAL + 13)

#pragma  endregion                    

#pragma region ������ش���           
//�������                            
#define EQERR_SOCKET_ERR              (EQERR_BASE_NET + 1)
//��������ʧ��                        
#define EQERR_CONNECT_FAIL            (EQERR_BASE_NET + 2)
//�������ӳ�ʱ                        
#define EQERR_CONNECT_TIMEOUT         (EQERR_BASE_NET + 3)
//�������ʱ���ӶϿ�
#define EQERR_RECVCONNECTION_CLOSED   (EQERR_BASE_NET + 4)
//���緢��ʧ��                        
#define EQERR_SENDSOCK_FAIL           (EQERR_BASE_NET + 5)
//���緢�ͳ�ʱ                        
#define EQERR_SENDSOCK_TIMEOUT        (EQERR_BASE_NET + 6)
//������մ���                        
#define EQERR_RECVSOCK_FAIL           (EQERR_BASE_NET + 7)
//������ճ�ʱ                        
#define EQERR_RECVSOCK_TIMEOUT        (EQERR_BASE_NET + 8)
#pragma  endregion

#pragma region �������������
//�������Ϊ��
#define EQERR_INPARAM_EMPTY       (EQERR_BASE_PARAM + 1)
//��������Ϊ��
#define EQERR_OUTPARAM_EMPTY      (EQERR_BASE_PARAM + 2)
//��������
#define EQERR_PARAM_ERR           (EQERR_BASE_PARAM + 3)
//��ʼ���ڸ�ʽ����ȷ
#define EQERR_START_DATE_ERR      (EQERR_BASE_PARAM + 4)
//��ֹ���ڸ�ʽ����ȷ  
#define EQERR_END_DATE_ERR        (EQERR_BASE_PARAM + 5)
//��ʼ���ڴ��ڽ�������
#define EQERR_START_BIGTHAN_END   (EQERR_BASE_PARAM + 6)
//���ڸ�ʽ����ȷ  
#define EQERR_DATE_ERR            (EQERR_BASE_PARAM + 7)
//��Ч��֤ȯ����
#define EQERR_CODE_INVALIED       (EQERR_BASE_PARAM + 8)
//֤ȯ�����ظ�
#define EQERR_CODE_REPEAT         (EQERR_BASE_PARAM + 9)
//��Ч��ָ��
#define EQERR_INDICATOR_INVALIED  (EQERR_BASE_PARAM + 10)
//�û���Ϊ��
#define EQERR_USERNAME_EMPTY      (EQERR_BASE_PARAM + 11)
//����Ϊ��
#define EQERR_PASSWORD_EMPTY      (EQERR_BASE_PARAM + 12)
//���������Ʊ�����ﵽ����
#define EQERR_TO_UPPER_LIMIT      (EQERR_BASE_PARAM + 13)
//��֧�ֵĻ��ָ��
#define EQERR_MIXED_INDICATOR     (EQERR_BASE_PARAM + 14)
//���ζ���ָ��ﵽ����
#define EQERR_INDICATOR_TO_UPPER_LIMIT  (EQERR_BASE_PARAM + 15)
//��������֧�ַ�Χ
#define EQERR_BEYOND_DATE_SUPPORT (EQERR_BASE_PARAM + 16)
//��Ȩ����С�ڽ�ֹ����
#define EQERR_BASE_LESS_THAN_END  (EQERR_BASE_PARAM + 17)
//��֧�ֵĻ��֤ȯƷ��
#define EQERR_MIXED_CODES_MARKET  (EQERR_BASE_PARAM + 18)
//��֧�ֵ�֤ȯ����Ʒ��
#define EQERR_NO_SUPPORT_CODES_MARKET  (EQERR_BASE_PARAM + 19)

#pragma  endregion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��������ö��
typedef enum
{
    eLang_ch,
    eLang_en,
} EQLang;

//EQVARTYPE��������ö��
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

// ����Events�ṹ���Ͷ���
typedef enum 
{
    eMT_err,                             // ������Ϣ
    eMT_response,                        // ���ݷ���
    eMT_partialResponse,                 // �������ݷ��أ�����ʱ���ã�
    eMT_others,                          // ������Ϣ
} EQMsgType;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//char�����װ�ṹ��
typedef struct _tagEQCHAR
{
    char* pChar;                                   //�ַ�����ָ��
    unsigned int nSize;                            //�ַ����鳤�ȣ�(�洢�ַ���ʱ����0��β���ȣ�
} EQCHAR;

//char��ά�����װ�ṹ��
typedef struct _tagEQCHARARRAY
{
    EQCHAR* pChArray;                              //EQCHAR����ָ��
    unsigned int nSize;                            //EQCHAR���鳤��
} EQCHARARRAY;

//EQVARIENT�������ͷ�װ�ṹ�壬�ɴ洢������������
typedef struct _tagEQVARIENT
{
    EQVarType vtype;                                //ֵ����
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
    } unionValues;                                   //������
    EQCHAR eqchar;                                   //�ַ�����
} EQVARIENT;

//EQVARIENT�������װ�ṹ��
typedef struct _tagEQVARIENTARRAY
{
    EQVARIENT* pEQVarient;                           //EQVARIENT����ָ��
    unsigned int nSize;                              //EQVARIENT���鳤��
} EQVARIENTARRAY;

//EQDATA���ݷ��ؽṹ��
typedef struct _tagEQDATA
{
    EQCHARARRAY codeArray;                           //��Ʊ��������
    EQCHARARRAY indicatorArray;                      //ָ������
    EQCHARARRAY dateArray;                           //��������
    EQVARIENTARRAY valueArray;                       //ֵ����

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

// ��¼��Ϣ
#define MAXSTRING 255
typedef struct _tagEQLOGININFO
{
    char userName[MAXSTRING];                        //��¼�û���
    char password[MAXSTRING];                        //��¼����
} EQLOGININFO;

//EQMSG���ݽṹ��(�첽���ػص�ʱʹ��)
typedef struct _tagEQMSG
{
    int version;							         //�汾��
    EQMsgType msgType;                               //Msg����
    EQErr err;							             //������
    EQID requestID;						             //����ID
    EQID serialID;						             //��ˮ��
    EQDATA* pEQData;      		                     //����������
} EQMSG, *PEQMSG;


//��������ṹ��
typedef struct _tagEQCtrData
{
	int            row;                              //����
	int            column;                           //����
	EQCHARARRAY    indicatorArray;                   //ָ��������
	EQVARIENTARRAY valueArray;                       //ֵ����

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

//��־�ص�����
typedef int (*logcallback)(const char* log);

//����ص�����(�첽����ʱʹ��)
typedef int (*datacallback) (const EQMSG* pMsg, LPVOID lpUserParam);

//�û������Զ���ServerList.json�ļ��Ĵ��·��
EMQUANTAPI void setserverlistdir(const char* dir);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ���͵�½(��ʼʱ����)  options�����Ӳ��� "TestLatency=1"
EMQUANTAPI EQErr start(EQLOGININFO* pLoginInfo, const char* options, logcallback pfnCallback);
//�˳�(����ʱ����)
EMQUANTAPI EQErr stop();
//�������ص���������start�����
EMQUANTAPI EQErr setcallback(datacallback pfnCallback);
//��ȡ�������ı�˵��
EMQUANTAPI const char* geterrstring(EQErr errcode, EQLang lang=eLang_en);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����˵��:
//    codes: ���ƴ���  ���������ð�Ƕ��Ÿ�����֧�ִ�Сд���� "300059.SZ,000002.SZ,000003.SZ,000004.SZ"
//    indicators: ����ָ�� ���ָ����ð�Ƕ��Ÿ�����֧�ִ�Сд���� "open,close,high"
//    startdate��enddate: ��ʼ���ںͽ�ֹ���ڡ����޷ָ����������Ϊ8λ���֡���ʽ֧��:YYYYMMDD YYYY/MM/DD YYYY/M/D YYYY-MM-DD YYYY-M-D
//    options: ���Ӳ���  ��������԰�Ƕ��Ÿ�����"Period=1,Market=CNSESH,Order=1,Adjustflag=1,Curtype=1,Pricetype=1,Type=1"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ָ��������ݲ�ѯ(ͬ������)
EMQUANTAPI EQErr csd(const char* codes, const char* indicators, const char* startDate, const char* endDate, const char* options, EQDATA*& pEQData);
//�������ݲ�ѯ(ͬ������)
EMQUANTAPI EQErr css(const char* codes, const char* indicators, const char* options, EQDATA*& pEQData);
//��ȡ���������ڵĽ�����(ͬ������)
EMQUANTAPI EQErr tradedates(const char* startDate, const char* endDate, const char* options, EQDATA*& pEQData);
//��ȡϵͳ���ɷ�(ͬ������)
EMQUANTAPI EQErr sector(const char* pukeyCode, const char* tradeDate, const char* options, EQDATA*& pEQData);
//��ȡƫ��N�Ľ�����(ͬ������)
#ifdef WIN32 
EMQUANTAPI EQErr getdate(const char* tradeDate, int offDay, const char* options, EQDATA*& pEQData);
#else
EMQUANTAPI EQErr gettradedate(const char* tradeDate, int offDay, const char* options, EQDATA*& pEQData);
#endif
//��ʷ����K��(ͬ������) //codeֻ֧�ֵ�����Ʊ
EMQUANTAPI EQErr csc(const char* code, const char* indicators, const char* startDate, const char* endDate, const char* options, EQDATA*& pEQData);
//�������(ͬ������) ÿ��indicators���Ϊ64��
EMQUANTAPI EQErr csqsnapshot(const char* codes, const char* indicators, const char* options, EQDATA*& pEQData);
//��ȡר�ⱨ��(ͬ������)
EMQUANTAPI EQErr ctr(const char* ctrName, const char* indicators, const char* options, EQCTRDATA*& pEQCtrData);
//������̬����ͬ���ӿڷ�������ָ���ͷ��ڴ�
EMQUANTAPI EQErr releasedata(void* pEQData);


//����Ϊ�첽����
//���أ� EQID  ��ˮ��
//pfnCallback����ͬ�Ľӿڿ����趨��ͬ�Ļص�����NULL��ʹ��Ĭ�ϵ����ص�����
//lpUserParam: �û�����,�ص�ʱԭ������

//ʵʱ����(�첽)  ÿ��indicators���Ϊ64�� options: Pushtype=0 ��������  1ȫ������
EMQUANTAPI EQID csq(const char* codes, const char* indicators, const char* options, datacallback pfnCallback, LPVOID lpUserParam);
//ȡ��ʵʱ���鶩��   serialIDΪ0ʱ ȡ�����ж���
EMQUANTAPI EQErr csqcancel(EQID serialID);

//�������۷���(�첽)  startdatetime��enddatetime��ʽ(YYYYMMDDHHMMSS��HHMMSS��ʾϵͳ���ڵ����ʱ�䣬������ʹ��ͬһ�ָ�ʽ)
EMQUANTAPI EQID cst(const char* codes, const char* indicators, const char* startdatetime, const char* enddatetime, const char* options, datacallback pfnCallback, LPVOID lpUserParam);

#endif /*#ifndef _EMQUANTAPI_H_*/
