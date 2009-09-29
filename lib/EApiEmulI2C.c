/*
 *<KHeader>
 *+=========================================================================
 *I               EApiDK Embedded Application Development Kit
 *+=========================================================================
 *I  $HeadURL$
 *+=========================================================================
 *I   Copyright: Copyright (c) 2002-2009, Kontron Embedded Modules GmbH
 *I      Author: John Kearney,                  John.Kearney@kontron.com
 *I
 *I     License: All rights reserved. This program and the accompanying 
 *I              materials are licensed and made available under the 
 *I              terms and conditions of the BSD License which 
 *I              accompanies this distribution. The full text of the 
 *I              license may be found at 
 *I              http://opensource.org/licenses/bsd-license.php
 *I              
 *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
 *I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
 *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
 *I
 *I Description: Auto Created for EApiEmulI2C.c
 *I
 *+------------------------------------------------------------------------=
 *I
 *I  File Name            : EApiEmulI2C.c
 *I  File Location        : lib
 *I  Last committed       : $Revision$
 *I  Last changed by      : $Author$
 *I  Last changed date    : $Date$
 *I  ID                   : $Id$
 *I
 *+=========================================================================
 *</KHeader>
 */

#include <EApiLib.h>

#define END_OF_LIST_MARK ((uint32_t)-1)


/*
 *
 *  
 *
 *      I2C  Emulation
 *
 *
 *
 */
#define I2C_EMUL_FREQ  100000
#define I2C_EMUL_BYTE_DELAY_NS (1000000000/(I2C_EMUL_FREQ/(8+1)))
typedef 
uint32_t 
I2CDevEmulator(
    __IN uint32_t Arg1, 
    __IN void *pWBuffer, 
    __IN uint32_t WriteBCnt, 
    __OUT void *pRBuffer, 
    __IN uint32_t ReadBCnt
    );

I2CDevEmulator EmulateDIDEeprom;
I2CDevEmulator EmulateUDSEeprom;
I2CDevEmulator EmulateEPIEeprom;
I2CDevEmulator EmulatePICMGEeprom;

typedef struct CmdDesc_s{
  uint32_t Cmd           ;/* Cmd */
  uint32_t CmdBlockLen   ;/* Cmd Buffer Length */
  void     *pBuffer      ;/* Cmd Content */ 
}CmdDesc_t;
typedef struct CmdDeviceCfgData_s{
  uint32_t ExtendedCmd   ;/* Ext/Std Index */
  uint32_t CurrentCmd    ;/* Ext/Std Index */
  uint32_t CurrentOffset ;/* Ext/Std Index */
  CmdDesc_t *Cmds        ;/* Commands Buffer pointer */ 
}CmdDeviceCfgData_t;

typedef struct EepromCfgData_s{
  uint32_t ByteLen       ;/* EEPROM Length */
  uint32_t ExtendedCmd   ;/* Ext/Std Index */
  uint32_t PageLength    ;/* Write Page Alignment size */
#define STD_INDEX 1                  
#define EXT_INDEX 2                  
  uint32_t CurIndx       ;/* Current Index Value */
  uint8_t  *DatapBuffer   ;/* Eeprom pBuffer Pointer */ 
}EepromCfgData_t;

typedef struct BusDevicesTbl_s{
  uint32_t EncAddr         ;/* Device Address */
  I2CDevEmulator *Emulator ;/* Emulator */
  uint32_t Arg1            ;/* Argument to pass to Emulator */
}BusDevicesTbl_t;

uint32_t 
EmulateEeprom(
    __IN uint32_t Arg1, 
    __IN EepromCfgData_t *CfgData, 
    __IN void *pWBuffer, 
    __IN uint32_t WriteBCnt, 
    __OUT void *pRBuffer, 
    __IN uint32_t ReadBCnt
    )
{
  
#if (STRICT_VALIDATION>1)
  EAPI_LIB_MSG_OUT(("O%04u %-30s : %-30s : ECMD=%1"PRIX32" CINDX=%04"PRIX32
        " Arg1=%1"PRIX32" DLEN=%04"PRIX32" PLEN=%04"PRIX32" WLEN=%04"PRIX32
        " RLEN=%04"PRIX32" %08"PRIX32"\n", __LINE__, "EmulateEeprom", 
        "Info", CfgData->ExtendedCmd, CfgData->CurIndx, Arg1, 
        CfgData->ByteLen, CfgData->PageLength, WriteBCnt, ReadBCnt, 
        ((uint32_t*)pWBuffer)[0])
      );
#endif
  if(WriteBCnt)
  {
    if((CfgData->ExtendedCmd==STD_INDEX))        /* Emulate Standard IDX/CMD 
                                                    Device */
    {
      CfgData->CurIndx=*(uint8_t*)pWBuffer;
      pWBuffer=&((uint8_t*)pWBuffer)[1];
      WriteBCnt--;
      CfgData->CurIndx|=Arg1<<8;                 /* Add the Device Address 
                                                    Index */
      EAPI_EMUL_DELAY_NS(I2C_EMUL_BYTE_DELAY_NS);
    }
    else if((CfgData->ExtendedCmd==EXT_INDEX))   /* Emulate Extended IDX/CMD
                                                    Device */
    {

      EAPI_LIB_RETURN_ERROR_IF(
          EmulateEeprom, 
          WriteBCnt<2, 
          EAPI_STATUS_READ_ERROR, 
          " STD INDEX Sent to EXT INDEX EEPROM"
          );

      CfgData->CurIndx =((uint8_t*)pWBuffer)[1]<<8;
      CfgData->CurIndx|=((uint8_t*)pWBuffer)[0];
      pWBuffer=&((uint8_t*)pWBuffer)[2];
      WriteBCnt-=2;
      CfgData->CurIndx|=Arg1<<16;                /* Add the Device Address Index */
      EAPI_EMUL_DELAY_NS(I2C_EMUL_BYTE_DELAY_NS*2);
    }
#if (STRICT_VALIDATION>1)
    EAPI_LIB_MSG_OUT(("O%04u %-30s : %-30s : CINDX=%04"PRIX32" PLEN=%04"PRIX32
          " EPLEN=%04"PRIX32"\n", __LINE__, "EmulateEeprom", "Info", 
          CfgData->CurIndx, CfgData->CurIndx%CfgData->PageLength, 
          (CfgData->CurIndx%CfgData->PageLength)+WriteBCnt)
        );
#endif
    if(WriteBCnt)
    {
      EAPI_LIB_RETURN_ERROR_IF(
          EmulateEeprom,
          ((CfgData->CurIndx%CfgData->PageLength)+WriteBCnt)>CfgData->PageLength,
          EAPI_STATUS_READ_ERROR, 
          "Device Page Wrap around"
          );
      EAPI_LIB_RETURN_ERROR_IF(
          EmulateEeprom,
          (CfgData->CurIndx+WriteBCnt)>CfgData->ByteLen   , 
          EAPI_STATUS_READ_ERROR, 
          "Device Write Wrap around"
          );

      EAPI_EMUL_DELAY_NS(I2C_EMUL_BYTE_DELAY_NS*WriteBCnt);
      memcpy(&CfgData->DatapBuffer[CfgData->CurIndx], pWBuffer, WriteBCnt);
      CfgData->CurIndx+=WriteBCnt;
    }
  }

  if(ReadBCnt)
  {
    EAPI_LIB_RETURN_ERROR_IF(
        EmulateEeprom,
        (CfgData->CurIndx+ReadBCnt)>CfgData->ByteLen      , 
        EAPI_STATUS_READ_ERROR, 
        "Device Read Wrap around"
        );

    EAPI_EMUL_DELAY_NS(I2C_EMUL_BYTE_DELAY_NS*ReadBCnt);
    memcpy(pRBuffer, &CfgData->DatapBuffer[CfgData->CurIndx], ReadBCnt);
    CfgData->CurIndx+=ReadBCnt;
  }
  EAPI_LIB_RETURN_SUCCESS(EmulateEeprom, "");
}

uint32_t 
EmulateCmdDevice(
    __IN uint32_t Arg1, 
    __IN CmdDeviceCfgData_t *CfgData, 
    __IN void *pWBuffer, 
    __IN uint32_t WriteBCnt, 
    __OUT void *pRBuffer, 
    __IN uint32_t ReadBCnt
    )
{
    int i;
    uint32_t ReadBCnt2;
  
#if (STRICT_VALIDATION>1)
  EAPI_LIB_MSG_OUT(("O%04u %-30s : %-30s : "
        " Arg1=%1"PRIX32" WLEN=%04"PRIX32
        " RLEN=%04"PRIX32" %08"PRIX32"\n", __LINE__, "EmulateCmdDevice", 
        "Info", Arg1, WriteBCnt, ReadBCnt, ((uint32_t*)pWBuffer)[0])
      );
#endif
  if(WriteBCnt)
  {
    if((CfgData->ExtendedCmd==STD_INDEX))        /* Emulate Standard IDX/CMD 
                                                    Device */
    {
      CfgData->CurrentCmd|=((uint8_t*)pWBuffer)[0];
      pWBuffer=&((uint8_t*)pWBuffer)[1];
      WriteBCnt--;
      EAPI_EMUL_DELAY_NS(I2C_EMUL_BYTE_DELAY_NS);
    }
    else if((CfgData->ExtendedCmd==EXT_INDEX))   /* Emulate Extended IDX/CMD
                                                    Device */
    {
      EAPI_LIB_RETURN_ERROR_IF(
          EmulateEeprom, 
          WriteBCnt<2, 
          EAPI_STATUS_WRITE_ERROR, 
          " STD INDEX Sent to EXT INDEX EEPROM"
          );

      CfgData->CurrentCmd =((uint8_t*)pWBuffer)[1]<<8;
      CfgData->CurrentCmd|=((uint8_t*)pWBuffer)[0];
      pWBuffer=&((uint8_t*)pWBuffer)[2];
      WriteBCnt-=2;
      EAPI_EMUL_DELAY_NS(I2C_EMUL_BYTE_DELAY_NS*2);
    }
    if(WriteBCnt)
    {
      EAPI_EMUL_DELAY_NS(I2C_EMUL_BYTE_DELAY_NS*WriteBCnt);
      for(i=0;CfgData->Cmds[i].Cmd!=END_OF_LIST_MARK;i++){
        if(CfgData->Cmds[i].Cmd==CfgData->CurrentCmd){
          ReadBCnt2=CfgData->Cmds[i].CmdBlockLen;
          memcpy(
            CfgData->Cmds[i].pBuffer, 
            pRBuffer, 
            (ReadBCnt>ReadBCnt2?ReadBCnt2:ReadBCnt)
            );
        }
      }
    }
  }
  if(ReadBCnt)
  {
    EAPI_EMUL_DELAY_NS(I2C_EMUL_BYTE_DELAY_NS*ReadBCnt);
    for(i=0;CfgData->Cmds[i].Cmd!=END_OF_LIST_MARK;i++){
      if(CfgData->Cmds[i].Cmd==CfgData->CurrentCmd){
        ReadBCnt2=CfgData->Cmds[i].CmdBlockLen;
        if(ReadBCnt2<ReadBCnt){
          memcpy(pRBuffer, CfgData->Cmds[i].pBuffer, ReadBCnt2);
          memset(pRBuffer, ReadBCnt-ReadBCnt2, 0xFF);
        }else{
          memcpy(pRBuffer, CfgData->Cmds[i].pBuffer, ReadBCnt);
        }
      }
    }
  }
  EAPI_LIB_RETURN_SUCCESS(EmulateEeprom, "");
}
#define bin_data DIDEEPROM
#include "DIDData.h"
#undef bin_data
static EepromCfgData_t DIDEEPromData={
  sizeof(DIDEEPROM),  /* EEPROM Length */
  EXT_INDEX        ,  /* Ext/Std Index */
  256              ,  /* Device Page Length */
  0                ,  /* Current Index Value */
  DIDEEPROM           /* Eeprom pBuffer Pointer */
};
uint32_t 
EmulateDIDEeprom(
    __IN uint32_t Arg1, 
    __IN void *pWBuffer, 
    __IN uint32_t WriteBCnt, 
    __OUT void *pRBuffer, 
    __IN uint32_t ReadBCnt
    )
{
  return EmulateEeprom(
      Arg1, 
      &DIDEEPromData, 
      pWBuffer, 
      WriteBCnt, 
      pRBuffer, 
      ReadBCnt
      );
}
#define bin_data UDSEEPROM
#include "UDSData.h"
#undef bin_data
static EepromCfgData_t UDSEEPromData={
  sizeof(UDSEEPROM),  /* EEPROM Length */
  STD_INDEX        ,  /* Ext/Std Index */
  16               ,  /* Device Page Length */
  0                ,  /* Current Index Value */
  UDSEEPROM           /* Eeprom pBuffer Pointer */
};
uint32_t 
EmulateUDSEeprom(
    __IN uint32_t Arg1, 
    __IN void *pWBuffer, 
    __IN uint32_t WriteBCnt, 
    __OUT void *pRBuffer, 
    __IN uint32_t ReadBCnt
    )
{
  return EmulateEeprom(
      Arg1, 
      &UDSEEPromData, 
      pWBuffer, 
      WriteBCnt, 
      pRBuffer, 
      ReadBCnt
      );
}
#define bin_data EPIEEPROM
#include "EPIData.h"
#undef bin_data
static EepromCfgData_t EPIEEPromData={
  sizeof(EPIEEPROM),  /* EEPROM Length */
  STD_INDEX        ,  /* Ext/Std Index */
  16               ,  /* Device Page Length */
  0                ,  /* Current Index Value */
  EPIEEPROM           /* Eeprom pBuffer Pointer */ 
};

uint32_t 
EmulateEPIEeprom(
    __IN uint32_t Arg1, 
    __IN void *pWBuffer, 
    __IN uint32_t WriteBCnt, 
    __OUT void *pRBuffer, 
    __IN uint32_t ReadBCnt
    )
{
  return EmulateEeprom(
      Arg1, 
      &EPIEEPromData, 
      pWBuffer, 
      WriteBCnt, 
      pRBuffer, 
      ReadBCnt
      );
}
/* #define bin_data PICMGEEPROM
 * #include "EPIData.h"
 * #undef bin_data
 */
uint8_t PICMGEEPROM[512]={0};
static EepromCfgData_t PICMGEEPromData={
  sizeof(PICMGEEPROM),  /* EEPROM Length */
  STD_INDEX          ,  /* Ext/Std Index */
  16                 ,  /* Max Block Write Length */
  0                  ,  /* Current Index Value */
  PICMGEEPROM           /* Eeprom pBuffer Pointer */ 
};

uint32_t 
EmulatePICMGEeprom(
    __IN uint32_t Arg1, 
    __IN void *pWBuffer, 
    __IN uint32_t WriteBCnt, 
    __OUT void *pRBuffer, 
    __IN uint32_t ReadBCnt
    )
{
  return EmulateEeprom(
      Arg1, 
      &PICMGEEPromData, 
      pWBuffer, 
      WriteBCnt, 
      pRBuffer, 
      ReadBCnt
      );
}

int8_t CpuTemp=56;
int8_t ChipsetTemp=46;
int8_t SystemTemp=-46;
CmdDesc_t HwmonCmdIndx[]={
  {0x0010, sizeof(CpuTemp)    , &CpuTemp    },
  {0x0011, sizeof(ChipsetTemp), &ChipsetTemp},
  {0x0012, sizeof(SystemTemp) , &SystemTemp },
  {END_OF_LIST_MARK, 0 , NULL },
};
CmdDeviceCfgData_t HWMon={
  STD_INDEX,
  0,
  0,
  HwmonCmdIndx
};
uint32_t 
EmulateHWMonDevice(
    __IN  uint32_t Arg1, 
    __IN  void    *pWBuffer, 
    __IN  uint32_t WriteBCnt, 
    __OUT void    *pRBuffer, 
    __IN  uint32_t ReadBCnt
    )
{
  return EmulateCmdDevice(
      Arg1, 
      &HWMon, 
      pWBuffer, 
      WriteBCnt, 
      pRBuffer, 
      ReadBCnt
      );
}

const BusDevicesTbl_t ExternalI2CDevices[]={
/* Device      Device         BLCK*/
/* Address    Emulator        CNT */
  {0x0052, EmulateHWMonDevice, 0},
  {0x00A6, EmulatePICMGEeprom, 0},
  {0x00A8, EmulatePICMGEeprom, 1},
  {END_OF_LIST_MARK, NULL, 0}
};
const BusDevicesTbl_t LVDS_1I2CDevices[]={
/* Device      Device         BLCK*/
/* Address    Emulator        CNT */
  {0x00A0, EmulateUDSEeprom, 0},
  {0x00A2, EmulateUDSEeprom, 1},
  {0x00A4, EmulateDIDEeprom, 0},
  {0xF255, EmulateDIDEeprom, 0},
  {END_OF_LIST_MARK, NULL, 0}
};
const BusDevicesTbl_t LVDS_2I2CDevices[]={
/* Device      Device         BLCK*/
/* Address    Emulator        CNT */
  {0x00A0, EmulateEPIEeprom, 0},
  {0x00A2, EmulateEPIEeprom, 1},
  {0x00A4, EmulateDIDEeprom, 0},
  {END_OF_LIST_MARK, NULL, 0}
};

typedef struct InterfaceIdsTbl_s{
  const uint32_t         Id         ; /* I2C EAPI Bus Id */
  const BusDevicesTbl_t *DevicesList; /* Pointer to Device List for 
                                       * Emulated I2C Bus */
  uint32_t               MaxBlockLen; /* Max Block Length Supported 
                                       * by Interface */
}InterfaceIdsTbl_t;

const InterfaceIdsTbl_t EmulatedInterfaces[]={
/*                                           Interface */
/*                                           Max Block */
/* BUS EApi ID           Bus Device List     Length    */
  {EAPI_ID_I2C_EXTERNAL, ExternalI2CDevices, 0x10000},
  {EAPI_ID_I2C_LVDS_1,   LVDS_1I2CDevices  , 0x00100},
  {EAPI_ID_I2C_LVDS_2,   LVDS_2I2CDevices  , 0x00020}
};
uint32_t 
EAPI_CALLTYPE 
EApiI2CGetBusCapEmul(
    __IN  uint32_t  Id, 
    __OUT uint32_t *pMaxBlkLen
    )
{
  uint32_t i;

  for(i=0;i<ARRAY_SIZE(EmulatedInterfaces);i++)
  {
      if(EmulatedInterfaces[i].Id==Id)
      {
          *pMaxBlkLen=EmulatedInterfaces[i].MaxBlockLen;
          EAPI_LIB_RETURN_SUCCESS(
            EApiI2CGetBusCapEmul, 
            ""
            );
      }
  }

  EAPI_LIB_RETURN_ERROR(
    EApiI2CGetBusCapEmul, 
    EAPI_STATUS_UNSUPPORTED  , 
    "Unrecognised I2C ID"
    );
}

/*
 *
 *  
 *
 *      I2C
 *
 *
 *
 */
uint32_t 
EAPI_CALLTYPE 
EApiI2CWriteReadEmul(
    __IN      uint32_t Id         , 
    __IN      uint8_t  Addr       , 
    __INOPT   void    *pWBuffer  , 
    __IN      uint32_t WriteBCnt  , 
    __OUTOPT  void    *pRBuffer  , 
    __IN      uint32_t ReadBCnt
    )
{
  uint32_t i,i2;
  uint32_t LclAddr;

  for(i=0;i<ARRAY_SIZE(EmulatedInterfaces);i++)
  {
      if(EmulatedInterfaces[i].Id==Id)
      {
	EAPI_LIB_RETURN_ERROR_IF(
	    EApiI2CWriteReadEmul, 
	    (WriteBCnt>EmulatedInterfaces[i].MaxBlockLen+1), 
	    EAPI_STATUS_INVALID_BLOCK_LENGTH,
	    "WriteBCnt>(pMaxBlkLen+1)"
	    );
	EAPI_LIB_RETURN_ERROR_IF(
	    EApiI2CWriteReadEmul, 
	    (ReadBCnt>(EmulatedInterfaces[i].MaxBlockLen+1)), 
	    EAPI_STATUS_INVALID_BLOCK_LENGTH,
	    "ReadBCnt>(pMaxBlkLen+1)"
	    );
        if(EAPI_I2C_IS_10BIT_ADDR(Addr<<8)){
          LclAddr=Addr<<8;
          LclAddr|=*((uint8_t *)pWBuffer);
          pWBuffer=((uint8_t *)pWBuffer)+1;
          WriteBCnt--;
        }
        else
          LclAddr=Addr;

#if (STRICT_VALIDATION>1)
        EAPI_LIB_MSG_OUT(("O%04u %-30s : %-30s : ADDR=%02"PRIX16
            " \n", __LINE__, "EApiI2CWriteReadEmul", "Info", LclAddr
            ));
#endif
        for(i2=0;
            EmulatedInterfaces[i].DevicesList[i2].EncAddr!=END_OF_LIST_MARK;
            i2++
            )
        {
          if(EmulatedInterfaces[i].DevicesList[i2].EncAddr==LclAddr){
            return EmulatedInterfaces[i].DevicesList[i2].Emulator(
                EmulatedInterfaces[i].DevicesList[i2].Arg1, 
                pWBuffer, 
                (WriteBCnt?WriteBCnt-1:0), 
                pRBuffer, 
                (ReadBCnt?ReadBCnt-1:0)
                );
          }
        }
        
        EAPI_LIB_RETURN_ERROR(
            EApiI2CWriteReadEmul, 
            EAPI_STATUS_NOT_FOUND  , 
            "Device Not Present"
            );
      }
  }

  EAPI_LIB_RETURN_ERROR(
      EApiI2CWriteReadEmul, 
      EAPI_STATUS_UNSUPPORTED  , 
      "Unrecognised I2C ID"
      );
}

