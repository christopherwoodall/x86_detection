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


#ifndef C1222CBC_H
#define C1222CBC_H

Boolean C1222CBCEncrypt(Unsigned8* data, Unsigned16 dataLength, Unsigned8* key, Unsigned16 keyLength, Unsigned8* iv, Unsigned8 ivLength, Unsigned8* keybuffer288,
             void (*keepAliveFunctionPtr)(void* p), void* keepAliveParam);
Boolean C1222CBCDecrypt(Unsigned8* data, Unsigned16 dataLength, Unsigned8* key, Unsigned16 keyLength, Unsigned8* iv, Unsigned8 ivLength, Unsigned8* keybuffer288,
             void (*keepAliveFunctionPtr)(void* p), void* keepAliveParam);


#endif
