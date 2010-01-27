/*
 *<KHeader>
 *+=========================================================================
 *I  Project Name: EApiDK Embedded Application Development Kit
 *+=========================================================================
 *I  $HeadURL$
 *+=========================================================================
 *I   Copyright: Copyright (c) 2002-2010, Kontron Embedded Modules GmbH
 *I      Author: John Kearney,                  John.Kearney@kontron.com
 *I
 *I     License: This program contains proprietary and confidential 
 *I              information. All rights reserved except as may be 
 *I              permitted by prior written consent.
 *I
 *I Description: Auto Created for sprintf.c
 *I
 *+-------------------------------------------------------------------------
 *I
 *I  File Name            : EeePArg.h
 *I  File Location        : apps\EeePProg
 *I  Last committed       : $Revision$
 *I  Last changed by      : $Author$
 *I  Last changed date    : $Date$
 *I  ID                   : $Id$
 *I
 *+=========================================================================
 *</KHeader>
 */
#ifndef __EEEPARG_H__
#define __EEEPARG_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct Options_s{
  unsigned int uiHelp;
  unsigned int uiQuiet;
  unsigned int uiVerbose;
  unsigned int uiCreateCOM0R20M_CFG;
  char  *szCOM0R20M_Cfg_File;
  unsigned int uiCreateCOM0R20M_IMG;
  char  *szCOM0R20M_SCfg_File;
  char  *szCOM0R20M_Img_File;
  unsigned int uiCreateCOM0R20M_EEP;
  char  *szCOM0R20M_S2Cfg_File;
  unsigned int uiCreateCOM0R20CB_CFG;
  char  *szCOM0R20CB_Cfg_File;
  unsigned int uiCreateCOM0R20CB_IMG;
  char  *szCOM0R20CB_SCfg_File;
  char  *szCOM0R20CB_Img_File;
  unsigned int uiCreateCOM0R20CB_EEP;
  char  *szCOM0R20CB_S2Cfg_File;
  unsigned int uiCreateEeePExpEEP_CFG;
  char  *szEeePExpEEP_Cfg_File;
  unsigned int uiCreateEeePExpEEP_IMG;
  char  *szEeePExpEEP_SCfg_File;
  char  *szEeePExpEEP_Img_File;
  unsigned int uiCreateEeePExpEEP_EEP;
  char  *szEeePExpEEP_S2Cfg_File;
  unsigned long  ulEeePExpEEP_Img_EApiBus;
  unsigned long  ulEeePExpEEP_Img_DevAddr;
  unsigned int uiProgramEeePExpEEP_EEP;
  char  *szEeePExpEEP_SBIN_File;
  unsigned long  ulEeePExpEEP_Bin_EApiBus;
  unsigned long  ulEeePExpEEP_Bin_DevAddr;
  unsigned int uiProgramCOM0R20CB_EEP;
  char  *szCOM0R20CB_SBIN_File;
  unsigned int uiProgramCOM0R20M_EEP;
  char  *szCOM0R20M_SBIN_File;
  unsigned int uiListDBlocks_IMG;
  char  *szGEN_List_DBlocks_File;
  unsigned int uiListDBlocks_EEP;
  unsigned long  ulEEP_Img_EApiBus;
  unsigned long  ulEEP_Img_DevAddr;
  unsigned int uiListDBlocks_COM0R20_CB_EEP;
  unsigned int uiListDBlocks_COM0R20_M_EEP;
  unsigned int uiStoreEeePExpEEP_EEP;
  char  *        szEeePExpEEP_To_Img_FName  ;
  unsigned long  ulEeePExpEEP_To_Img_EApiBus;
  unsigned long  ulEeePExpEEP_To_Img_DevAddr;
  unsigned int uiStoreCOM0R20CB_EEP;
  char  *        szCOM0R20CBEEP_To_Img_FName  ;
  unsigned int uiStoreCOM0R20M_EEP;
  char  *        szCOM0R20MEEP_To_Img_FName  ;
}Options_t;

extern Options_t CurOptions;

#ifdef __cplusplus
}
#endif
#endif /* __EEEPARG_H__ */
