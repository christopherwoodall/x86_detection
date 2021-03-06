// CDDL HEADER START
//*************************************************************************************************
//                                     Copyright � 2006-2009
//                                           Itron, Inc.
//                                      All Rights Reserved.
//
//
// The contents of this file, and the files included with this file, are subject to the current 
// version of Common Development and Distribution License, Version 1.0 only (the �License�), which 
// can be obtained at http://www.sun.com/cddl/cddl.html. You may not use this file except in 
// compliance with the License. 
//
// The original code, and all software distributed under the License, are distributed and made 
// available on an �AS IS� basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED. ITRON, 
// INC. HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR PARTICULAR PURPOSE, OR NON INFRINGEMENT. Please see the License 
// for the specific language governing rights and limitations under the License.
//
// When distributing Covered Software, include this CDDL Header in each file and include the 
// License file at http://www.sun.com/cddl/cddl.html.  If applicable, add the following below this 
// CDDL HEADER, with the fields enclosed by brackets �[   ]� replaced with your own identifying 
// information: 
//		Portions Copyright [yyyy]  [name of copyright owner]
//
//*************************************************************************************************
// CDDL HEADER END

#include "c1222stack.h"



// host implementation


Unsigned32 receivedMessages = 0;
Unsigned32 receivedRegistrations = 0;



Unsigned8 hostAddress[6] = { 0x11, 0x22, 0x33, 0x44, 0, 0 };

Boolean HostMessageIsRegistration(ACSE_Message* pMsg)
{
    Epsem epsem;
    Unsigned8* buffer;
    Unsigned16 length;
    Unsigned16 tagIndex;
    Unsigned16 tagLength;
    Unsigned8* data;
    
    if ( ACSE_Message_GetApData(pMsg, &data, &tagLength) )
    {
        Epsem_Init(&epsem, data, tagLength);
        
        if ( Epsem_Validate(&epsem) )
        {
            while( Epsem_GetNextRequestOrResponse( &epsem, &buffer, &length) )
            {
                if ( buffer[0] == 0x27 )
                    return TRUE;
            }
        }
    }
    
    return FALSE;
}




void HostMakeRegistrationResponse(ACSE_Message* pMsg, ACSE_Message* pResponseMsg)
{
    //C1222ApTitle apTitle;
    static Unsigned8 localBuffer[100];
    static Unsigned8 myNodeApTitle[] = { 0x0d, 0x03, 0x44, 0x21, 0x03 }; // .68.33.3 - Aptitle - Mentioned in readme.txt *. This aptitle will later be set to the device in c1222test.c
    static Unsigned8 myMRApTitle[] = { 0x0d, 0x02, 0x44, 0x21 }; //Master Relay aptitle
    //static Unsigned8 myRelayAddress[] = { 0x01, 0x02, 0x03, 0x05, 0, 0 };
    //static Unsigned16 regDelay = 0x1234;
    //static Unsigned32 regPeriod = 0x300000;
    //static Unsigned8 regFlag = 1;
    
    Unsigned16 sequence;

    if ( !ACSE_Message_GetCallingApInvocationId(pMsg, &sequence) )
        sequence = 0;
    
    // for now just make up a response
    
    if ( pMsg->standardVersion == CSV_MARCH2006 )
    {
        localBuffer[0] = 0x60;
        localBuffer[1] = 37; // length
        
        localBuffer[2] = 0xA2;
        localBuffer[3] = 5;
        memcpy(&localBuffer[4], myNodeApTitle, 5);
        
        localBuffer[9] = 0xA6;
        localBuffer[10] = 4;
        memcpy(&localBuffer[11], myMRApTitle, 4);
        
        localBuffer[15] = 0xA8;
        localBuffer[16] = 2;
        localBuffer[17] = 0;
        localBuffer[18] = 11;
        
        localBuffer[19] = 0xA4;
        localBuffer[20] = 2;
        localBuffer[21] = (Unsigned16)(sequence >> 8);
        localBuffer[22] = (Unsigned16)(sequence & 0xFF); // need a called ap invocation id equal to calling from msg
        
        localBuffer[23] = 0xBE;
        localBuffer[24] = 14; // length
        localBuffer[25] = 0x80;
        localBuffer[26] = 12;// length
        localBuffer[27] = 0x00;
        memcpy(&localBuffer[28], myNodeApTitle, 5);
        localBuffer[33] = 0x12;
        localBuffer[34] = 0x34;
        localBuffer[35] = 0;
        localBuffer[36] = 0;
        localBuffer[37] = 0;
        localBuffer[38] = 1;
        
        pResponseMsg->buffer = &localBuffer[2];
        pResponseMsg->length = 37;
        pResponseMsg->maxLength = 37;
    }
    else
    {
        localBuffer[0] = 0x60; //ACSE identifier
        localBuffer[1] = 44; // length of whole ACSE message
        
        localBuffer[2] = 0xA2; // Called Aptitle element or destination*
        localBuffer[3] = 5; // Called Aptitle length
        memcpy(&localBuffer[4], myNodeApTitle, 5);
        if ( myNodeApTitle[0] == 0x0d )
            localBuffer[4] = 0x80;
            
        localBuffer[9] = 0xA4;//called-AP-invocation-id-element
        localBuffer[10] = 4;//called-AP-invocation-id-integerlength
        localBuffer[11] = 0x02;//called-AP-invocation-id-integer
        localBuffer[12] = 2;//called-AP-invocation-id-length
        localBuffer[13] = (Unsigned16)(sequence >> 8);//called-AP-invocation-id
        localBuffer[14] = (Unsigned16)(sequence & 0xFF); // need a called ap invocation id equal to calling from msg //called-AP-invocation-id

        localBuffer[15] = 0xA6; // Calling Aptitle element or initiator*
        localBuffer[16] = 4; // Calling Aptitle length
        memcpy(&localBuffer[17], myMRApTitle, 4);
        if ( myMRApTitle[0] == 0x0d )
            localBuffer[17] = 0x80;
            
        localBuffer[21] = 0xA8;//calling-AP-invocation-id-element
        localBuffer[22] = 3; // Its length
        localBuffer[23] = 0x02; //calling-AP-invocation-id-integer
        localBuffer[24] = 1; //calling-AP-invocation-id-length
        localBuffer[25] = 11; //calling-AP-invocation-id
        
        localBuffer[26] = 0xBE; // User Information element
        localBuffer[27] = 19; // length
        localBuffer[28] = 0x28;//user-information-external
        localBuffer[29] = 17;//user-information-length
        localBuffer[30] = 0x81;//user-information-octet-string
        localBuffer[31] = 15;//user-information-octet-string-length
        localBuffer[32] = 0x80;// this is epsem control
        localBuffer[33] = 12;// length of epsem data
        localBuffer[34] = 0x00;// Response ok (EPSEM)
        memcpy(&localBuffer[35], myNodeApTitle, 5);
        localBuffer[40] = 0x12; //reg-delay
        localBuffer[41] = 0x34; // reg-delay
        localBuffer[42] = 0;// Registration is always valid . 42,43 and 44
        localBuffer[43] = 0;
        localBuffer[44] = 0;
        localBuffer[45] = 1; // DIRECT_MESSAGING_AVAILABLE.
        
        pResponseMsg->buffer = &localBuffer[2];
        pResponseMsg->length = 44;
        pResponseMsg->maxLength = 44;
    }

    (void)pMsg;

}



Boolean simulateHost(ACSE_Message* pMsg, C1222Stack* pStack, Boolean * pMessageSent)
{
    ACSE_Message responseMessage;
    static Unsigned8 responseBuffer[1000];  // static to keep it off the stack

    receivedMessages++;

    *pMessageSent = FALSE;

    ACSE_Message_SetBuffer(&responseMessage, responseBuffer, sizeof(responseBuffer));
    responseMessage.standardVersion = pMsg->standardVersion;

    if ( HostMessageIsRegistration(pMsg) )
    {
        HostMakeRegistrationResponse(pMsg, &responseMessage);
        
        C1222Stack_NoteReceivedMessageHandlingIsComplete(pStack);

        printf("Sending registration response\n");

        *pMessageSent = C1222Stack_SendMessage(pStack, &responseMessage, hostAddress, sizeof(hostAddress)); // this routine is what is called by the stack to send a message.
    // the stack is not aware of who the message is for
    // if this is the comm module, this could be a registration response, in which case we just need
    // to handle it. If it is already not in registering state, it will send the response to the hostAddress. If it is already in the registering state, the info from the response	
	// will be saved (In this case, since it is a response, the comm module would already have been in the registering state because it would have sent a 
	// registration request in the past. Since it is in the registering state, it will just store the contents of the response)
	//These are then later obtained by the register Device by calling DeviceRefreshStatistics.

        receivedRegistrations++;
        
        return TRUE;
    }
    
    return FALSE;
}







Boolean CMsimulateHost(ACSE_Message* pMsg, C1222Stack* pStack, Boolean * pMessageSent, Boolean sendRegistrationToNetwork)
{
    ACSE_Message responseMessage;
    static Unsigned8 responseBuffer[1000];  // static to keep it off the stack
    Unsigned8* nativeAddress;
    Unsigned8 addressLength;
    
    *pMessageSent = FALSE;

    receivedMessages++;

    ACSE_Message_SetBuffer(&responseMessage, responseBuffer, sizeof(responseBuffer));
    responseMessage.standardVersion = pMsg->standardVersion;

    if ( HostMessageIsRegistration(pMsg) && !sendRegistrationToNetwork )
    {
        if ( !C1222Stack_GetReceivedMessageNativeAddress(pStack, &addressLength, &nativeAddress) )
        {
            printf("CM can't get native address\n");
            return FALSE;
        }

        printf("CM Sending registration response\n");
        
        HostMakeRegistrationResponse(pMsg, &responseMessage);
        
        C1222Stack_NoteReceivedMessageHandlingIsComplete(pStack);
        

        *pMessageSent = C1222Stack_SendMessage(pStack, &responseMessage, nativeAddress, addressLength);

        receivedRegistrations++;
        
        return TRUE;
    }
    
    return FALSE;
}



Unsigned32 receivedMessages_for_Client = 0;
Unsigned32 receivedRegistrations_for_Client = 0;




void HostMakeRegistrationResponse_for_Client(ACSE_Message* pMsg, ACSE_Message* pResponseMsg)
{
    //C1222ApTitle apTitle;
    static Unsigned8 localBuffer[100];
    static Unsigned8 myNodeApTitle[] = { 0x0d, 0x03, 0x44, 0x21, 0x10 }; // .68.33.16 - Aptitle . This aptitle will later be set to the device in c1222test_client.c
    static Unsigned8 myMRApTitle[] = { 0x0d, 0x02, 0x44, 0x21 }; //Master Relay aptitle. This will later be set to the MRAptitle field in c1222test_client.c
    //static Unsigned8 myRelayAddress[] = { 0x01, 0x02, 0x03, 0x05, 0, 0 };
    //static Unsigned16 regDelay = 0x1234;
    //static Unsigned32 regPeriod = 0x300000;
    //static Unsigned8 regFlag = 1;
    
    Unsigned16 sequence;

    if ( !ACSE_Message_GetCallingApInvocationId(pMsg, &sequence) )
        sequence = 0;
    
    // for now just make up a response
    
    if ( pMsg->standardVersion == CSV_MARCH2006 )
    {
        localBuffer[0] = 0x60;
        localBuffer[1] = 37; // length
        
        localBuffer[2] = 0xA2;
        localBuffer[3] = 5;
        memcpy(&localBuffer[4], myNodeApTitle, 5);
        
        localBuffer[9] = 0xA6;
        localBuffer[10] = 4;
        memcpy(&localBuffer[11], myMRApTitle, 4);
        
        localBuffer[15] = 0xA8;
        localBuffer[16] = 2;
        localBuffer[17] = 0;
        localBuffer[18] = 11;
        
        localBuffer[19] = 0xA4;
        localBuffer[20] = 2;
        localBuffer[21] = (Unsigned16)(sequence >> 8);
        localBuffer[22] = (Unsigned16)(sequence & 0xFF); // need a called ap invocation id equal to calling from msg
        
        localBuffer[23] = 0xBE;
        localBuffer[24] = 14; // length
        localBuffer[25] = 0x80;
        localBuffer[26] = 12;// length
        localBuffer[27] = 0x00;
        memcpy(&localBuffer[28], myNodeApTitle, 5);
        localBuffer[33] = 0x12;
        localBuffer[34] = 0x34;
        localBuffer[35] = 0;
        localBuffer[36] = 0;
        localBuffer[37] = 0;
        localBuffer[38] = 1;
        
        pResponseMsg->buffer = &localBuffer[2];
        pResponseMsg->length = 37;
        pResponseMsg->maxLength = 37;
    }
    else
    {
        localBuffer[0] = 0x60; //ACSE identifier
        localBuffer[1] = 44; // length of whole ACSE message
        
        localBuffer[2] = 0xA2; // Called Aptitle element or destination*
        localBuffer[3] = 5; // Called Aptitle length
        memcpy(&localBuffer[4], myNodeApTitle, 5);
        if ( myNodeApTitle[0] == 0x0d )
            localBuffer[4] = 0x80;
            
        localBuffer[9] = 0xA4;//called-AP-invocation-id-element
        localBuffer[10] = 4;//called-AP-invocation-id-integerlength
        localBuffer[11] = 0x02;//called-AP-invocation-id-integer
        localBuffer[12] = 2;//called-AP-invocation-id-length
        localBuffer[13] = (Unsigned16)(sequence >> 8);//called-AP-invocation-id
        localBuffer[14] = (Unsigned16)(sequence & 0xFF); // need a called ap invocation id equal to calling from msg //called-AP-invocation-id

        localBuffer[15] = 0xA6; // Calling Aptitle element or initiator*
        localBuffer[16] = 4; // Calling Aptitle length
        memcpy(&localBuffer[17], myMRApTitle, 4);
        if ( myMRApTitle[0] == 0x0d )
            localBuffer[17] = 0x80;
            
        localBuffer[21] = 0xA8;//calling-AP-invocation-id-element
        localBuffer[22] = 3; // Its length
        localBuffer[23] = 0x02; //calling-AP-invocation-id-integer
        localBuffer[24] = 1; //calling-AP-invocation-id-length
        localBuffer[25] = 11; //calling-AP-invocation-id
        
        localBuffer[26] = 0xBE; // User Information element
        localBuffer[27] = 19; // length
        localBuffer[28] = 0x28;//user-information-external
        localBuffer[29] = 17;//user-information-length
        localBuffer[30] = 0x81;//user-information-octet-string
        localBuffer[31] = 15;//user-information-octet-string-length
        localBuffer[32] = 0x80;// this is epsem control
        localBuffer[33] = 12;// length of epsem data
        localBuffer[34] = 0x00;// Response ok (EPSEM)
        memcpy(&localBuffer[35], myNodeApTitle, 5);
        localBuffer[40] = 0x12; //reg-delay
        localBuffer[41] = 0x34; // reg-delay
        localBuffer[42] = 0;// Registration is always valid . 42,43 and 44
        localBuffer[43] = 0;
        localBuffer[44] = 0;
        localBuffer[45] = 1; // DIRECT_MESSAGING_AVAILABLE.
        
        pResponseMsg->buffer = &localBuffer[2];
        pResponseMsg->length = 44;
        pResponseMsg->maxLength = 44;
    }

    (void)pMsg;

}



Boolean simulateHost_for_Client(ACSE_Message* pMsg, C1222Stack* pStack, Boolean * pMessageSent)
{
    ACSE_Message responseMessage;
    static Unsigned8 responseBuffer[1000];  // static to keep it off the stack

    receivedMessages++;

    *pMessageSent = FALSE;

    ACSE_Message_SetBuffer(&responseMessage, responseBuffer, sizeof(responseBuffer));
    responseMessage.standardVersion = pMsg->standardVersion;

    if ( HostMessageIsRegistration(pMsg) )
    {
        HostMakeRegistrationResponse_for_Client(pMsg, &responseMessage);
        
        C1222Stack_NoteReceivedMessageHandlingIsComplete(pStack);

        printf("Sending registration response\n");

        *pMessageSent = C1222Stack_SendMessage(pStack, &responseMessage, hostAddress, sizeof(hostAddress)); // this routine is what is called by the stack to send a message.
    // the stack is not aware of who the message is for
    // if this is the comm module, this could be a registration response, in which case we just need
    // to handle it. If it is already not in registering state, it will send the response to the hostAddress. If it is already in the registering state, the info from the response	
	// will be saved (In this case, since it is a response, the comm module would already have been in the registering state because it would have sent a 
	// registration request in the past. Since it is in the registering state, it will just store the contents of the response)
	//These are then later obtained by the register Device by calling DeviceRefreshStatistics.

        receivedRegistrations_for_Client++;
        
        return TRUE;
    }
    
    return FALSE;
}







Boolean CMsimulateHost_for_Client(ACSE_Message* pMsg, C1222Stack* pStack, Boolean * pMessageSent, Boolean sendRegistrationToNetwork)
{
    ACSE_Message responseMessage;
    static Unsigned8 responseBuffer[1000];  // static to keep it off the stack
    Unsigned8* nativeAddress;
    Unsigned8 addressLength;
    
    *pMessageSent = FALSE;

    receivedMessages++;

    ACSE_Message_SetBuffer(&responseMessage, responseBuffer, sizeof(responseBuffer));
    responseMessage.standardVersion = pMsg->standardVersion;

    if ( HostMessageIsRegistration(pMsg) && !sendRegistrationToNetwork )
    {
        if ( !C1222Stack_GetReceivedMessageNativeAddress(pStack, &addressLength, &nativeAddress) )
        {
            printf("CM can't get native address\n");
            return FALSE;
        }

        printf("CM Sending registration response\n");
        
        HostMakeRegistrationResponse_for_Client(pMsg, &responseMessage);
        
        C1222Stack_NoteReceivedMessageHandlingIsComplete(pStack);
        

        *pMessageSent = C1222Stack_SendMessage(pStack, &responseMessage, nativeAddress, addressLength);

        receivedRegistrations_for_Client++;
        
        return TRUE;
    }
    
    return FALSE;
}

