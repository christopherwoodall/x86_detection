#include "c1222stack.h"
#include "c1219table.h"
#include "epsem.h"
#include "acsemsg.h"

#define REQUEST_TYPE_IDENT 1
#define REQUEST_TYPE_READ 2
#define REQUEST_TYPE_WRITE 3
#define REQUEST_TYPE_LOGON 4
#define REQUEST_TYPE_LOGOFF 5
#define REQUEST_TYPE_TERMINATE 6
#define REQUEST_TYPE_DISCONNECT 7
#define REQUEST_TYPE_WAIT 8
#define REQUEST_TYPE_RESOLVE 9
#define REQUEST_TYPE_TRACE 10
#define MAX_EPSEM_BODY_SIZE 100
#define MAX_ACSE_MSG_HDR_SIZE 100
#define MAX_ACSE_MSG_SIZE 200

typedef struct C1222SClientTag
{
    C1222Stack* pStack;
       
    
    Unsigned8* requestMsgBuffer;
	Unsigned8* responseMsgbuffer;
	Unsigned8* Called_Aptitle;
	Unsigned8 Called_Aptitle_length;
    Unsigned8* myAptitle;
	Unsigned8 myAptitle_length;
	Unsigned8* myMRAptitle;
	Unsigned8 myMRAptitle_length;
    Unsigned16 CallingAp_InvocationId;
	Unsigned8* Destination_Native_Address;
	Unsigned16 Destination_Native_Address_length;
    
} C1222Client;