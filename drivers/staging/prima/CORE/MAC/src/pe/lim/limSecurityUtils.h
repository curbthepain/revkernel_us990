/*
 * Copyright (c) 2012-2013 The Linux Foundation. All rights reserved.
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * This file was originally distributed by Qualcomm Atheros, Inc.
 * under proprietary terms before Copyright ownership was assigned
 * to the Linux Foundation.
 */

/*
  
                                                                
                                            
                                  
                          
            
                                                         
                                                                       
 */
#ifndef __LIM_SECURITY_UTILS_H
#define __LIM_SECURITY_UTILS_H
#include "sirMacProtDef.h" //                        

#define LIM_ENCR_AUTH_BODY_LEN  (sizeof(tSirMacAuthFrameBody) + \
                                SIR_MAC_WEP_IV_LENGTH + \
                                SIR_MAC_WEP_ICV_LENGTH)
struct tLimPreAuthNode;

tANI_U8        limIsAuthAlgoSupported(tpAniSirGlobal, tAniAuthType, tpPESession);

//                                           
void               limInitPreAuthList(tpAniSirGlobal);
void               limDeletePreAuthList(tpAniSirGlobal);
struct tLimPreAuthNode    *limSearchPreAuthList(tpAniSirGlobal, tSirMacAddr);
void               limAddPreAuthNode(tpAniSirGlobal, struct tLimPreAuthNode *);
void               limDeletePreAuthNode(tpAniSirGlobal, tSirMacAddr);
void               limReleasePreAuthNode(tpAniSirGlobal pMac, tpLimPreAuthNode pAuthNode);
void               limRestoreFromAuthState(tpAniSirGlobal,
                                           tSirResultCodes, tANI_U16,tpPESession);

//                                        
tCfgWepKeyEntry    *limLookUpKeyMappings(tSirMacAddr);
void               limComputeCrc32(tANI_U8 *, tANI_U8 *, tANI_U8);
void               limRC4(tANI_U8 *, tANI_U8 *, tANI_U8 *, tANI_U32, tANI_U16);
void               limEncryptAuthFrame(tpAniSirGlobal, tANI_U8, tANI_U8 *, tANI_U8 *, tANI_U8 *, tANI_U32);
tANI_U8                 limDecryptAuthFrame(tpAniSirGlobal, tANI_U8 *, tANI_U8 *, tANI_U8 *, tANI_U32, tANI_U16);

void limSendSetBssKeyReq( tpAniSirGlobal, tLimMlmSetKeysReq *,tpPESession );
void limSendSetStaKeyReq( tpAniSirGlobal, tLimMlmSetKeysReq *, tANI_U16, tANI_U8,tpPESession);
void limPostSmeSetKeysCnf( tpAniSirGlobal, tLimMlmSetKeysReq *, tLimMlmSetKeysCnf * );

void limSendRemoveBssKeyReq(tpAniSirGlobal pMac, tLimMlmRemoveKeyReq * pMlmRemoveKeyReq,tpPESession);
void limSendRemoveStaKeyReq(tpAniSirGlobal pMac, tLimMlmRemoveKeyReq * pMlmRemoveKeyReq, tANI_U16 staIdx,tpPESession);
void limPostSmeRemoveKeyCnf(tpAniSirGlobal pMac, tpPESession psessionEntry, tLimMlmRemoveKeyReq * pMlmRemoveKeyReq, tLimMlmRemoveKeyCnf * mlmRemoveKeyCnf);

#define  PTAPS  0xedb88320

static inline tANI_U32
limCrcUpdate(tANI_U32 crc, tANI_U8 x)
{

    //                                                 
    //
    tANI_U32 z;
    tANI_U32 fb;
    int i;

    z = crc^x;
    for (i=0; i<8; i++) {
        fb = z & 1;
        z >>= 1;
        if (fb) z ^= PTAPS;
    }
    return z;
}

#endif /*                        */