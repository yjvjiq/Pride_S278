//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : ÇàÄêÆÖ½­6123
//* File Name          : Fault.c
//* Author             : SLX
//* Version            : V1.0.0
//* Start Date         : 2016.11.30
//* Description        : ¸ÃÎÄ¼ş ¶ÔÏµÍ³µÄ¸÷¼¶¹ÊÕÏÅĞ¶Ï
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h"
//#include "Fault.h"
#include "TotalVoltageOverVoltage.h"
#include "TotalVoltageUnderVoltage.h"
#include "DisChargeOverCurrent.h"
#include "ChargeOverCurrent.h"
#include "CellOverVoltage.h"
#include "CellUnderVoltage.h"
#include "CurrentSensorFault.h"
#include "CellVolUnbalance.h"
#include "BatteryTemperatureLow.h"
#include "BatteryTemperatureHigh.h"
#include "socLow.h"
#include "IsolationLow.h"
#include "ChargeSocketOverTemp.h"
#include "Supply24V.h"

///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 

//***********************************************************************************
//***********************************************************************************
//typedef struct{
	/* g_caution_Flag_1 */
//	U8 Pack_UV_Flt_lv3		:1; //bit0
//	U8 CellUV_Flt_lv3		:1;	//bit1
//	U8 SystemVoltageOV		:1;
//	U8 CellVoltageOV		:1;
//	U8 LowIsolation			:1;
//	U8 CellTemperatureOT	:1;
//	U8 CellTemperatureUT	:1;
//	U8 ChargeOC				:1;	//bit7

//	
//}CAUTION_FLAG_TYPE;

unsigned char	g_caution_Flag_1 = 0;//BMS¹ÊÕÏ×´Ì¬ FOR ÉÏÎ»»ú
unsigned char	g_caution_Flag_2 = 0;
unsigned char	g_caution_Flag_3 = 0;
unsigned char	g_caution_Flag_4 = 0;
unsigned int	g_errorCounter;
unsigned char	ACCha_Flag_BST = 0;
unsigned char	ACCOverTime = 0;//½»Á÷³äµç»úÍ¨ĞÅ¹ÊÕÏ
unsigned char	VCUOverTime = 0;//ÓëVCUÍ¨Ñ¶¹ÊÕÏ
unsigned char	SBMSOverTime = 0;//ÓëÁíÒ»Ö÷°åSBMSÍ¨Ñ¶¹ÊÕÏ
//unsigned char CarErrorLevel = 0;

CUTDISCURT0  CutDisCurt0;
CUTDISCURT50 CutDisCurt50;
//CUTDISCURT70 CutDisCurt70;

CUTCHACURT0 CutChaCurt0;
CUTCHACURT50 CutChaCurt50;
//CUTCHACURT70 CutChaCurt70;

CUTDCCHACURT0 CutDCChaCurt0;
CUTDCCHACURT50 CutDCChaCurt50;
//CUTDCCHACURT70 CutDCChaCurt70;

CUTACCHACURT0 CutACChaCurt0;
CUTACCHACURT50 CutACChaCurt50;
//CUTACCHACURT70 CutACChaCurt70;

unsigned char test1=0;

//declaration for the function in fault.c below
int TurnOffAllRelay(void);


//******************************************************************************
//* Function name:    ErrorToCarPC
//* Description:     ²»Í¬¹ÊÕÏµÄ´¦Àí·½Ê½
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
/*void ErrorToCarPC(unsigned char Err1,unsigned char Err2,unsigned char Err3,unsigned char Err4,unsigned char *CarError,
                  unsigned char *PCErrorLevel1,unsigned char *PCErrorLevel2, unsigned char *PCErrorLevel3)//
{
    if(Err1==1)//Èç¹û1¼¶¹ÊÕÏ 
    {
        *CarError=1;
        *PCErrorLevel1 |= 0x04;//to PC
    } 
    else//Ã»ÓĞ1¼¶¹ÊÕÏ
    {           
        if(Err1==1)//Èç¹ûÓĞ¶ş¼¶¹ÊÕÏ
        {
            *CarError=2;
            *PCErrorLevel2=1;//to PC
        } 
        else//Èç¹ûÃ»ÓĞ¶ş¼¶¹ÊÕÏ
        {
            *CarError=0;
            *PCErrorLevel2=0;//to PC                      
            if(Err1==1)//Èç¹ûÓĞÈı¼¶¹ÊÕÏ
            {
                *CarError=3;
                *PCErrorLevel3=1;//to PC 
            } 
            else//Èç¹ûÈı¼¶ÎŞ¹ÊÕÏ
            {
                *CarError=0;
                *PCErrorLevel3=0;//to PC 
                if(Err1==1)//4¼¶¹ÊÕÏ
                    *CarError = 4;                
                else  //ÎŞ4¼¶
                    *CarError = 0;                              
            }
        }
    }
} */
//******************************************************************************
//* Function name:    errorSystemVoltageOV
//* Description:      µç³Ø°ü×ÜµçÑ¹¹ıÑ¹¹ÊÕÏ 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorSystemVoltageOV(void) 
{
    unsigned char Error[4]={0};
    unsigned char i;
    unsigned char Level=0; 
	
    //judge by different BMS work mode, as the input is different.
    if(stateCode == 30)//discharge mode.
        Level=TotalVoltageOverVoltage_custom(g_systemVoltage,g_highVoltageV3,DISCHARGING);
           
    else if(stateCode == 170)//fast charge mode.
        Level=TotalVoltageOverVoltage_custom(g_systemVoltage,g_highVoltageV5,FASTRECHARGING);
    
    else if(stateCode == 110) // ACC charge mode.(re-charging mode)
        Level=TotalVoltageOverVoltage_custom(g_systemVoltage,g_highVoltageV6,RECHARGING);
	
    ///////////////////The fault level process and send out/////////////////////////   
    for(i=1;i<4;i++) 
       if(i==Level) 
           Error[i]=1;
       
    Error_Group4.Bit.F6_Bat_Over_V = Level;//send to vehicle CAN.
    
	//level 1 process
	Can554Byte2.Bit.F2_systemOV1 				= Error[1]; // send to PC software.
    
    //level 2 process
    Can554Byte0.Bit.F2_systemOV2				= Error[2]; // send to PC software.
    CutChaCurt50.Bit.F0_Battery_Over_Voltage2	= Error[2];
    CutDCChaCurt0.Bit.F0_Battery_Over_Voltage21	= Error[2];
    CutACChaCurt0.Bit.F0_Battery_Over_Voltage21	= Error[2];
    
    //level 3 process
    if(Error[3] == 1)
        g_caution_Flag_1 |= 0x04;// 0000 0100
    else
    {
        if(stateCode == 30){
            g_caution_Flag_1 &= 0xfb;//ffff 1011
        }
    }
    CutChaCurt0.Bit.F0_Battery_Over_Voltage1	= Error[3];
    CutDCChaCurt0.Bit.F0_Battery_Over_Voltage21	= Error[3];
    CutACChaCurt0.Bit.F0_Battery_Over_Voltage21	= Error[3];
}
//******************************************************************************
//* Function name:    errorSystemVoltageUV
//* Description:      µç³Ø°ü×ÜµçÑ¹Ç·Ñ¹¹ÊÕÏ 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorSystemVoltageUV(void) //»Ö¸´,ÉÏ±¨
{
    unsigned char i;
    unsigned char Error[4]={0};
    unsigned char Level=0; 
    ///////////////////ÉÏ±¨¹ÊÕÏµÈ¼¶Êı/////////////////////////
    
    Level = TotalVoltageUnderVoltage_custom(g_systemVoltage,g_highVoltageV3); 
    
    for(i=1;i<4;i++) 
       if(i==Level) 
           Error[i]=1;
       
    Error_Group4.Bit.F4_Bat_Under_V = Level;//Õû³µCAN¸³Öµ
    
    //1¼¶¹ÊÕÏ´¦Àí
    Can554Byte2.Bit.F0_systemUV1=Error[1]; //to PC
    
    //2¼¶´¦Àí
    Can554Byte0.Bit.F0_systemUV2=Error[2];//to PC
    CutDisCurt50.Bit.F0_Battery_Under_Voltage2=Error[2];
    
    //3¼¶´¦Àí 
    if(Error[3] == 1) 
    {
        g_caution_Flag_1 |= 0x01;
        status_group1.Bit.St_BMS = 2;//BMS×´Ì¬¸ßÑ¹¶Ï¿ª
    }
    CutDisCurt0.Bit.F0_Battery_Under_Voltage1 = Error[3];
}

//******************************************************************************
//* Function name:    errorDischargeOC
//* Description:      ·Åµç¹ıÁ÷¹ÊÕÏ  Ò»¼¶
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorDischargeOC(void) //»Ö¸´;ÉÏ±¨²»´¦Àí 
{
    unsigned char Error[4]={0};
    unsigned char i;
    unsigned char Level=0; 
    float curtValue=0;

    curtValue = BiggestDischargeCurt;
    
    ////////////////////////ÉÏ±¨¹ÊÕÏµÈ¼¶////////////////////////
    Level=DisChargeOverCurrent_custom(g_systemCurrent,curtValue);  
    for(i=1;i<4;i++) 
        if(i==Level) 
            Error[i]=1;
     
    Error_Group3.Bit.F6_DisChg_Over_I=Level;//Õû³µCAN¸³Öµ 
    
//	//1¼¶¹ÊÕÏ´¦Àí
	Can554Byte3.Bit.F0_DisChaOCurt1=Error[1];
//	CutDisCurt70.Bit.F1_DisCharge_Over_Current3=Error[1];

//	//2¼¶¹ÊÕÏ´¦Àí
	Can554Byte1.Bit.F0_DisChaOCurt2=Error[2];//to PC
//	CutDisCurt50.Bit.F1_DisCharge_Over_Current2=Error[2];

//	//3¼¶¹ÊÕÏ´¦Àí
//	if(Error[3]) 
//    {
//        g_caution_Flag_2 |= 0x01;//to PC
//        //status_group2.Bit.BMS_PowerOff_Req = 2;//BMS¸ßÑ¹ÏÂµçÇëÇó
//    }

//	CutDisCurt0.Bit.F2_DisCharge_Over_Current1=Error[3];
}
//******************************************************************************
//* Function name:    errorChargeOC
//* Description:      ³äµç¹ıÁ÷¹ÊÕÏºÍ»ØÀ¡¹ıÁ÷¹ÊÕÏÍ¬Ò»Ä£ĞÍ 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorChargeOC(void)
{
    unsigned char Error[4]={0};
    unsigned char i;
    unsigned char Level=0; 
    float curtValue=0;
    
    if(g_BmsModeFlag == DISCHARGING)//·ÅµçÄ£Ê½
    {
        curtValue = BiggestFeedbackCurt; //»ØÀ¡µçÁ÷Îª¸ºÖµ
    }
    else if((g_BmsModeFlag == FASTRECHARGING)||(g_BmsModeFlag == RECHARGING)) //¿ì³äÄ£Ê½
    {
        curtValue = m_askcurrent;
    } 
    ///////////////////ÉÏ±¨¹ÊÕÏµÈ¼¶Êı/////////////////////////    
    Level=ChargeOverCurrent_custom((-g_systemCurrent),curtValue);    
    for(i=1;i<4;i++) 
        if(i==Level) 
            Error[i]=1;         
    
    if(g_BmsModeFlag == DISCHARGING)
        Error_Group5.Bit.F0_FeedB_Over_I = Level;//Õû³µCAN¸³Öµ,Ë²Ê±¹¦ÂÊ
    else
    {
        Error_Group1.Bit.F4_Ch_Over_I = Level;//Õû³µCAN¸³Öµ,³äµç¹¦ÂÊ
        
    }    
    if(Level>=1){
        Error_Group6.Bit.F5_Chg_C_Over = 1;//Õû³µCAN¸³Öµ,³äµçµçÁ÷³¬ÏŞ±¨¾¯
    }
    else{
        Error_Group6.Bit.F5_Chg_C_Over = 0;//Õû³µCAN¸³Öµ,³äµçµçÁ÷³¬ÏŞ±¨¾¯
    }
	
//	//1¼¶¹ÊÕÏ´¦Àí
	Can554Byte2.Bit.F7_ChangerOCurt1=Error[1];//to PC
//	CutChaCurt70.Bit.F1_Charge_Over_Current3=Error[1];
//	CutDCChaCurt70.Bit.F1_Charge_Over_Current3=Error[1];
//	CutACChaCurt70.Bit.F1_Charge_Over_Current3=Error[1];
    
//	//2¼¶¹ÊÕÏ´¦Àí
	Can554Byte0.Bit.F7_ChangerOCurt2=Error[2];//to PC
//	CutChaCurt50.Bit.F1_Charge_Over_Current2=Error[2];
//	CutDCChaCurt50.Bit.F0_Charge_Over_Current2=Error[2];
//	CutACChaCurt50.Bit.F0_Charge_Over_Current2=Error[2];
    
//	//3¼¶¹ÊÕÏ´¦Àí
//	if(Error[3])
//	{
//		g_caution_Flag_1 |=0x80;//to PC
//		if(g_BmsModeFlag == DISCHARGING){
//			status_group2.Bit.BMS_PowerOff_Req = 2;//BMS¸ßÑ¹ÏÂµçÇëÇó
//		}
//	 }

//	CutChaCurt0.Bit.F2_Charge_Over_Current1=Error[3];
//	CutDCChaCurt0.Bit.F3_Charge_Over_Current1=Error[3];
//	CutACChaCurt0.Bit.F3_Charge_Over_Current1=Error[3];
}

//******************************************************************************
//* Function name:    errorCellVoltageOV
//* Description:      µ¥Ìå¹ıÑ¹¹ÊÕÏ,1¼¶¹ÊÕÏ²»»Ö¸´,2,3,4¼¶¹ÊÕÏ¿É»Ö¸´,Ö±Á÷³ıÍâ,ÈôÖ±Á÷ÉÏ±¨2¼¶¹ÊÕÏ,¿ÉÒÔµ½1Ò²¿ÉÒÔµ½3,Èç¹û1¼¶Ö±½Ó²»»Ö¸´  
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorCellVoltageOV(void)
{
    unsigned char Error[4]={0};
    unsigned char i;
    unsigned char Level=0;
	
    //ÉÏ±¨¹ÊÕÏµÈ¼¶µÈ¼¶Êı 
    Level = CellOverVoltage_custom(g_highestCellVoltage,g_BmsModeFlag);      
    for(i=1;i<4;i++){
       if(i == Level){
           Error[i] = 1;
       }
    }
    
    Error_Group2.Bit.F0_Cell_Over_V = Level;//will be send to vehicle CAN    
    
    //level 1 process
    Can554Byte2.Bit.F3_cellOV1					= Error[1];
//	CutChaCurt70.Bit.F2_Cell_Over_Voltage3		= Error[1];
	CutACChaCurt0.Bit.F4_Cell_Over_Voltage321	= Error[1];
	CutDCChaCurt0.Bit.F4_Cell_Over_Voltage321	= Error[1];
	
	//level 2 process
	Can554Byte0.Bit.F3_cellOV2					= Error[2];//to PC
	CutChaCurt50.Bit.F2_Cell_Over_Voltage2		= Error[2];
	CutDCChaCurt0.Bit.F4_Cell_Over_Voltage321	= Error[2];
	CutACChaCurt0.Bit.F4_Cell_Over_Voltage321	= Error[2];
	
	//level 3 process
	if(Error[3])
	{
	    g_caution_Flag_1 |= 0x08;//to PC
	    if(g_BmsModeFlag == DISCHARGING){
	        status_group1.Bit.St_BMS = 2;//BMS staus is HV OFF
	    }
	}
	CutDisCurt0.Bit.F10_Cell_Over_Voltage1		= Error[3];
	CutChaCurt0.Bit.F3_Cell_Over_Voltage1		= Error[3];
	CutACChaCurt0.Bit.F4_Cell_Over_Voltage321	= Error[3];
	CutDCChaCurt0.Bit.F4_Cell_Over_Voltage321	= Error[3];
}
//******************************************************************************
//* Function name:   errorCellVoltageUV
//* Description:     µ¥ÌåµçÑ¹Ç·Ñ¹¹ÊÕÏ  
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorCellVoltageUV(void)//ÉÏ±¨²»´¦Àí,³äµç²»ÉÏ±¨,»Ö¸´
{
    unsigned char i;
    unsigned char Error[4]={0};
    unsigned char Level=0; 
	
    Level=CellUnderVoltage_custom(g_lowestCellVoltage);
    for(i=1;i<4;i++){
        if(i==Level){
            Error[i]=1;
        }
    }
	
    Error_Group2.Bit.F2_Cell_Under_V=Level;//Õû³µCAN¸³Öµ
    
    //1¼¶¹ÊÕÏ´¦Àí
    Can554Byte2.Bit.F1_cellUV1=Error[1];//to PC
//	CutDisCurt70.Bit.F2_Cell_Under_Voltage3=Error[1];
	
    //2¼¶¹ÊÕÏ´¦Àí
	Can554Byte0.Bit.F1_cellUV2=Error[2];//to PC
	CutDisCurt50.Bit.F2_Cell_Under_Voltage2=Error[2];
	 
	//3¼¶¹ÊÕÏ´¦Àí
	if(Error[3])
	{
		g_caution_Flag_1 |= 0x02;//to PC
		status_group1.Bit.St_BMS =2;//BMS×´Ì¬¸ßÑ¹¶Ï¿ª
	}
	CutDisCurt0.Bit.F3_Cell_Under_Voltage1 = Error[3];
	CutChaCurt0.Bit.F10_Cell_Under_Voltage1 = Error[3];
}

//******************************************************************************
//* Function name:    errorCurrSensor
//* Description:      µçÁ÷´«¸ĞÆ÷¹ÊÕÏ 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
Bool errorCurrSensorIniatial(void) //ÉÏµçÇ°¼ì²â2´Î
{
    unsigned char i;
    unsigned char Error[4]={0};
    unsigned char Level=0; 

    Level=CurrentSensorFault_custom(g_systemCurrent);
    for(i=1;i<4;i++)
        if(i==Level)
            Error[i]=1;
        
    if(Error[1]){
        g_caution_Flag_3 |=0x02; //to PC      //µçÁ÷´«¸ĞÆ÷¹ÊÕÏ
	}
	
    //hardware_error2.Bit.F7_I_Ga_Err = 1;
//	if(g_BmsModeFlag == DISCHARGING) 
//	{
//	    CutDisCurt0.Bit.F4_I_Sener_Err = 1;
//	    CutChaCurt0.Bit.F4_I_Sener_Err = 1;
//	    status_group2.Bit.BMS_PowerOff_Req = 2;//BMS¸ßÑ¹ÏÂµçÇëÇó 
//	}
//	else if(g_BmsModeFlag == FASTRECHARGING)
//	    CutDCChaCurt0.Bit.F5_I_Sener_Err =1;
//	else if(g_BmsModeFlag == RECHARGING)
//	    CutACChaCurt0.Bit.F5_I_Sener_Err =1;
//	           
//	}   
    return 0;  
}

//******************************************************************************
//* Function name:    errorCurrSensor
//* Description:      ÎÂ¶È´«¸ĞÆ÷¹ÊÕÏ 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************

//******************************************************************************
//* Function name:    errorCellUnbalance
//* Description:    µ¥ÌåµçÑ¹²»¾ùºâ¹ÊÕÏ 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorCellUnbalance(void)//
{
	unsigned char Error[4]={0};
	unsigned char i;
	unsigned char Level=0;

	//ÉÏ±¨¹ÊÕÏµÈ¼¶     
	Level=CellVolUnbalance_custom(g_highestCellVoltage,g_lowestCellVoltage);
	for(i=1;i<4;i++){
       if(i==Level){
           Error[i]=1;
       }
	}
	   
	Error_Group5.Bit.F4_Cell_Dif_V = Level;//Õû³µCAN¸³Öµ 
	
	//2¼¶¹ÊÕÏ´¦Àí
	Can554Byte1.Bit.F2_CellUnbalance2		= Error[2];//to PC
	CutDisCurt50.Bit.F3_CellUnbalance2		= Error[2];
	CutChaCurt50.Bit.F6_CellUnbalance2		= Error[2];
	CutDCChaCurt50.Bit.F6_CellUnbalance2	= Error[2];
	CutACChaCurt50.Bit.F5_CellUnbalance2	= Error[2];
}

//******************************************************************************
//* Function name:    errorTemUnbalance
//* Description:    µ¥ÌåÎÂ¶È²»¾ùºâ¹ÊÕÏ 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorTemUnbalance(void)//
{
	unsigned char Error[4]={0};
	unsigned char i;
	unsigned char Level;
	//float Tmp_H,Tmp_L;
	//Tmp_H=(float)g_highestTemperature;
	//Tmp_L=(float)g_lowestTemperature;

    //ÉÏ±¨¹ÊÕÏµÈ¼¶
    Level = CellTempUnbalance_custom(g_highestTemperature,g_lowestTemperature);
	
    for(i=1;i<4;i++){
       if(i==Level){
           Error[i]=1;
       }
    }
	
    Error_Group5.Bit.F2_Cell_Dif_T = Level;//Õû³µCAN¸³Öµ 
	
	//1¼¶¹ÊÕÏ´¦Àí
	Can554Byte3.Bit.F3_tempUnbalance1=Error[1];//to PC
//	CutDisCurt70.Bit.F3_tempUnbalance3=Error[3];
//	CutChaCurt70.Bit.F3_tempUnbalance3=Error[3];
//	CutDCChaCurt70.Bit.F2_tempUnbalance3=Error[3];
//	CutACChaCurt70.Bit.F2_tempUnbalance3=Error[3];

	//2¼¶¹ÊÕÏ´¦Àí
	Can554Byte1.Bit.F3_tempUnbalance2		= Error[2];//to PC
	CutDisCurt50.Bit.F4_tempUnbalance2		= Error[2];
	CutChaCurt50.Bit.F3_tempUnbalance2		= Error[2];
	CutDCChaCurt50.Bit.F1_tempUnbalance2	= Error[2];
	CutACChaCurt50.Bit.F1_tempUnbalance2	= Error[2];
}

//******************************************************************************
//* Function name:   errorCellTemperatureUT
//* Description:     µ¥ÌåÎÂ¶È¹ıµÍ¹ÊÕÏ 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorCellTemperatureUT(void)//
{
	unsigned char i;
	unsigned char Error[4]={0}; 
	unsigned char Level=0;
	//float t;
	//t= (float)g_lowestTemperature;

	//ÉÏ±¨¹ÊÕÏµÈ¼¶
	Level=BatteryTemperatureLow_custom(g_lowestTemperature);
	for(i=1;i<4;i++){
       if(i==Level){
           Error[i]=1;
       }
    }
    
	Error_Group5.Bit.F6_Cell_Under_T=Level;//Õû³µCAN¸³Öµ
	//1¼¶¹ÊÕÏ´¦Àí
	Can554Byte2.Bit.F6_cellUT1=Error[1];

	//2¼¶¹ÊÕÏ´¦Àí
	Can554Byte0.Bit.F6_cellUT2=Error[2];
    
	 //3¼¶¹ÊÕÏ´¦Àí
	if(Error[3]) 
	{
		g_caution_Flag_1 |= 0x40;//to PC
		if(g_BmsModeFlag == DISCHARGING){
			status_group1.Bit.St_BMS =2;//BMS×´Ì¬¸ßÑ¹¶Ï¿ª
		}
	}
	CutDisCurt0.Bit.F6_Under_Temp1		= Error[3];
	CutChaCurt0.Bit.F6_Under_Temp1		= Error[3];
	CutDCChaCurt0.Bit.F7_Under_Temp1	= Error[3];
	CutACChaCurt0.Bit.F7_Under_Temp1	= Error[3];
}

//******************************************************************************
//* Function name:    errorCellTemperatureOT
//* Description:      µ¥ÌåÎÂ¶È¹ı¸ß¹ÊÕÏ  Ò»¼¶
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorCellTemperatureOT(void)//
{
	unsigned char i;
	unsigned char Error[4]={0}; 
	unsigned char Level=0; 
	//float t;
	//t= (float)g_highestTemperature;

	//ÉÏ±¨¹ÊÕÏµÈ¼¶
	Level=BatteryTemperatureHigh_custom(g_highestTemperature);

	for(i=1;i<4;i++){
       if(i==Level){
           Error[i]=1;
       }
    }
    
	Error_Group2.Bit.F4_Temp_Over = Level;//Õû³µCAN¸³Öµ

	//1¼¶¹ÊÕÏ´¦Àí
	Can554Byte2.Bit.F5_cellOT1=Error[1];
//	CutDisCurt70.Bit.F4_Over_Temp3=Error[1];
//	CutChaCurt70.Bit.F4_Over_Temp3=Error[1];
//	CutDCChaCurt70.Bit.F3_Over_Temp3=Error[1];
//	CutACChaCurt70.Bit.F3_Over_Temp3=Error[1];
	
	//2¼¶¹ÊÕÏ´¦Àí
	Can554Byte0.Bit.F5_cellOT2=Error[2];//to PC
	CutDisCurt50.Bit.F5_Over_Temp2=Error[2];
	CutChaCurt50.Bit.F4_Over_Temp2=Error[2];
	CutDCChaCurt50.Bit.F2_Over_Temp2=Error[2];
	CutACChaCurt50.Bit.F2_Over_Temp2=Error[2];
    
    //3¼¶¹ÊÕÏ´¦Àí
    if(Error[3])
    {  
        g_caution_Flag_1 |= 0x20;//to PC
        if(g_BmsModeFlag == DISCHARGING)
            status_group1.Bit.St_BMS =2;//BMS×´Ì¬¸ßÑ¹¶Ï¿ª
    }
    CutDisCurt0.Bit.F5_Over_Temp1=Error[3];
    CutChaCurt0.Bit.F5_Over_Temp1=Error[3];
    CutDCChaCurt0.Bit.F6_Over_Temp1=Error[3];
    CutACChaCurt0.Bit.F6_Over_Temp1=Error[3];
}

//******************************************************************************
//* Function name:    errorSOCLow
//* Description:      SOCµÍ¹ÊÕÏ Ò»¼¶
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorSOCLow(void)
{
	unsigned char i;
	unsigned char Error[4]={0}; 
	unsigned char Level=0;
	//float t;

	//ÉÏ±¨¹ÊÕÏµÈ¼¶
	Level=socLow_custom(Can_g_socValue);
	for(i=1;i<4;i++) 
       if(i==Level) 
           Error[i]=1;
       
	Error_Group4.Bit.F0_SOC_Low=Level;//Õû³µCAN¸³Öµ 
	//1¼¶¹ÊÕÏ´¦Àí
	Can554Byte3.Bit.F1_SOCLow1=Error[1];

	//2¼¶¹ÊÕÏ´¦Àí
	Can554Byte1.Bit.F1_SOCLow2=Error[2];
}

//******************************************************************************
//* Function name:    errorLowIsolation
//* Description:      ¾øÔµµç×è¹ıµÍ¹ÊÕÏ 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorLowIsolation(void) 
{
	unsigned char i;
	unsigned char Error[4]={0};
	unsigned char Level=0; 
	float LowInsolation;
	
	if(Rn_Vpn_Value <= Rp_Vpn_Value)
        LowInsolation = Rn_Vpn_Value;
    else
        LowInsolation = Rp_Vpn_Value;
    
	Level=IsolationLow_custom(LowInsolation);
	for(i=1;i<4;i++)
      if(i==Level)
        Error[i]=1;
      
    Error_Group1.Bit.F6_Ins_Err=Level;//Õû³µCAN¸³Öµ
    if(Level>=1)
        Error_Group6.Bit.F7_Chg_Ins_Low = 1;//³äµç¾øÔµ¹ıµÍ±¨¾¯
	
    //1¼¶¹ÊÕÏ´¦Àí
    Can554Byte2.Bit.F4_insulationLow1=Error[1];//to PC
//	CutDCChaCurt70.Bit.F4_Low_Isolation3=Error[1]; //Ã»ÓĞ70½µÁË£¬Ö»ÉÏ±¨²»´¦Àí 
//	CutACChaCurt70.Bit.F4_Low_Isolation3=Error[1];
	
//	CutACChaCurt0.Bit.F8_Low_Isolation1=Error[2];//Âı³ä²»¼ì²â
    
    //2¼¶¹ÊÕÏ´¦Àí
    Can554Byte0.Bit.F4_insulationLow2=Error[2];//to PC
    CutDCChaCurt50.Bit.F3_Low_Isolation2=Error[2];
//	CutACChaCurt50.Bit.F3_Low_Isolation2=Error[2];
	
    //3¼¶¹ÊÕÏ´¦Àí
    if(Error[3])
        g_caution_Flag_1 |= 0x10;//to PC
	CutDCChaCurt0.Bit.F8_Low_Isolation1=Error[3];
}

//******************************************************************************
//* Function name:    DCChangerComError
//* Description:      ÓëÖ±Á÷³äµç»úÍ¨ĞÅ¹ÊÕÏ£¨°üÀ¨³¬Ê±¹ÊÕÏ£©
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void DCChangerComError(void)
{
	if(OverTimeState==1)
	{
		CutDCChaCurt0.Bit.F1_Communication_With_Charger=1;//¹ÊÕÏ¶¯×÷
		g_caution_Flag_2 |=0x80; //for ÄÚ²¿CAN
		Error_Group3.Bit.F1_V_CAN_Err =1; //Õû³µCAN
	}
}
//******************************************************************************
//* Function name:    ACChangerComError
//* Description:      ÓëÊÜµç¹­³äµç»úÍ¨ĞÅ¹ÊÕÏ£¨°üÀ¨³¬Ê±¹ÊÕÏ£©
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void ACChangerComError(void)
{
    ACCOverTime++;
    if(ACCOverTime >= 30)//30sÃ»ÓĞ½ÓÊÕµ½³äµç»ú±¨ÎÄ
    {
        CutACChaCurt0.Bit.F1_Communication_With_Charger=1;
        g_caution_Flag_2 |=0x10; //for ÄÚ²¿CAN
        Error_Group3.Bit.F1_V_CAN_Err =1; //Õû³µCAN
    }
}
//******************************************************************************
//* Function name:    VCUComError
//* Description:      ÓëÕû³µÍ¨ĞÅ¹ÊÕÏ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void VCUComError(void)
{
	VCUOverTime++;
	if(VCUOverTime>=30)//30sÃ»ÓĞ½ÓÊÕµ½³äµç»ú±¨ÎÄ
	{
		CutDisCurt0.Bit.F7_VCU_Communiction=1;
		CutChaCurt0.Bit.F7_VCU_Communiction=1;
		//g_caution_Flag_2 |=0x10; //for ÄÚ²¿CAN
		Error_Group3.Bit.F1_V_CAN_Err =1; //Õû³µCAN
		status_group1.Bit.St_BMS =2;//BMS×´Ì¬¸ßÑ¹¶Ï¿ª
	}
}
//******************************************************************************
//* Function name:    innerCommOT3
//* Description:      ÄÚ²¿Í¨Ñ¶¹ÊÕÏ  Èı¼¶
//* EntryParameter : None
//* ReturnValue    : BoolÀàĞÍ 0-ÎŞ£»1-ÓĞ
//******************************************************************************
void innerCommOT3(void) 
{ 
    SBMSOverTime++;
    if(((g_caution_Flag_3 & 0x01)!=0)||(SBMSOverTime>=30)) //BMUÄÚ²¿Í¨Ñ¶¹ÊÕÏ»òÓëSBMS¹ÊÕÏ
    {
        status_group1.Bit.St_BMS =2;//BMS×´Ì¬¸ßÑ¹¶Ï¿ª
        CutDisCurt0.Bit.F1_Inner_Communiction=1;
        CutChaCurt0.Bit.F1_Inner_Communiction=1;
        CutDCChaCurt0.Bit.F2_Inner_Communiction=1;
        CutACChaCurt0.Bit.F2_Inner_Communiction=1;
        
        Error_Group3.Bit.F0_Sub_Com_Err=1;//Õû³µCAN¸³Öµ 
    } 
}

//******************************************************************************
//* Function name:    Charge_Check
//* Description:      µçÁ÷Õı¸º¼ì²â 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void Charge_Check(void) 
{
	static unsigned char Check_Num=0;
	
	if(((g_BmsModeFlag == FASTRECHARGING)||(g_BmsModeFlag == RECHARGING))&&(g_systemCurrent > 10)) 
	{
		Check_Num++;
		if(Check_Num>=100) 
		{
			ACCha_Flag_BST=1;
			CutDCChaCurt0.Bit.F9_Charge_Count1=1;
			CutACChaCurt0.Bit.F9_Charge_Count1=1;
			Can554Byte3.Bit.F4_ChargeCurDirError = 1;
			if(Check_Num>=250) //Èç¹û·¢ÉúÁË100´Î£¬¸ÃËã·¨ÒªÇó±ØĞëÏÂµç¡£
				Check_Num=101;
        }
    }
}

//******************************************************************************
//* Function name:    CHG_SocketOT
//* Description:      ³äµç²å×ù¹ıÎÂ 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void CHG_SocketOT(void) 
{
    unsigned char i;
    unsigned char Error[4]={0}; 
    unsigned char Level=0;
    float tmax;
    
    if(DCTem1 >= DCTem2)
        tmax = DCTem1;
    else
        tmax = DCTem2;
    
   
    //ÉÏ±¨¹ÊÕÏµÈ¼¶
    if(tmax >= 0){ //±ØĞë´óÓÚ0
        Level = ChargeSocketOverTemp_custom((unsigned char)tmax);
    }
	else{ 
        Level = 0;
	}
	
    for(i=1;i<4;i++) 
       if(i == Level) 
           Error[i] = 1;
       
    Error_Group0.Bit.F2_Ele_Relay_Con = Level;//Õû³µCAN¸³Öµ     

    ///////2¼¶¹ÊÕÏ´¦Àí
    CutDCChaCurt50.Bit.F5_CHG_Socket2 = Error[2];
    
    /////3¼¶¹ÊÕÏ´¦Àí 
    CutDCChaCurt0.Bit.F12_CHG_Socket1 = Error[3];
}
//******************************************************************************
//* Function name:    Fire_Warning
//* Description:      »ğÔÖÔ¤¾¯ 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void Fire_Warning(void) 
{
    if(Error_Group0.Bit.F0_Fire_Warning==2) 
    {
        CutDisCurt50.Bit.F6_Fire_Warning2=1;
        CutChaCurt50.Bit.F5_Fire_Warning2=1;
        CutDCChaCurt50.Bit.F4_Fire_Warning2=1;
        CutACChaCurt50.Bit.F4_Fire_Warning2=1;
    } 
    else if(Error_Group0.Bit.F0_Fire_Warning==3) 
    {   
        CutDisCurt0.Bit.F8_Fire_Warning1=1;
        CutChaCurt0.Bit.F8_Fire_Warning1=1;
        CutDCChaCurt0.Bit.F10_Fire_Warning1=1;
        CutACChaCurt0.Bit.F10_Fire_Warning1=1;
        if(g_BmsModeFlag == DISCHARGING)
            status_group1.Bit.St_BMS =2;//BMS×´Ì¬¸ßÑ¹¶Ï¿ª
    }
}

//******************************************************************************
//* Function name:    PowerSupplyError
//* Description:      ¹©µçµçÔ´Òì³£
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void PowerSupplyError(void)//
{
    unsigned char i;
    unsigned char Error[4]={0}; 
    unsigned char Level=0;

    //ÉÏ±¨¹ÊÕÏµÈ¼¶
    
    Level=Supply24V_custom(PowerVOL);
    for(i=1;i<4;i++) 
       if(i==Level) 
           Error[i]=1;
    
    //1¼¶¹ÊÕÏ´¦Àí ÉÏ±¨¹ÊÕÏ  
    if(Error[1]) 
    {
        //Error_Group6.Bit.F0_Power_Vol=1;//Õû³µCANÉÏ±¨
        /*CutDisCurt0.Bit.F9_Supply_Power1=1;
        CutChaCurt0.Bit.F9_Supply_Power1=1;
        CutDCChaCurt0.Bit.F11_Supply_Power1=1;
        CutACChaCurt0.Bit.F11_Supply_Power1=1;
        */
    } 
}
//**********************************************************************
//* Function name:   CarFaultDone
//* Description:     ĞĞ³µ¹ı³ÌÖĞ¹ÊÕÏµÄ´¦Àí
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void CarFaultDone()
{
    //////////////////////////////ĞĞ³µ·¢ËÍ¸ßÑ¹ÏÂµçÇëÇó/////////////////////
    
    if(g_BmsModeFlag == DISCHARGING) //ĞĞ³µ·¢ËÍ¸ßÑ¹ÏÂµçÇëÇó
    {
        if((Error_Group4.Bit.F4_Bat_Under_V ==3)//×ÜµçÑ¹¹ıÑ¹3¼¶
        //||(Error_Group3.Bit.F6_DisChg_Over_I == 3) //·Åµç¹ıÁ÷3¼¶
        //||(Error_Group5.Bit.F0_FeedB_Over_I == 3)  //»ØÀ¡¹ıÁ÷3¼¶
        ||(Error_Group2.Bit.F0_Cell_Over_V==3)      //µ¥ÌåµçÑ¹¹ıÑ¹3¼¶
        ||(Error_Group2.Bit.F2_Cell_Under_V == 3)   //µ¥ÌåµçÑ¹Ç·Ñ¹3¼¶
        ||(Error_Group5.Bit.F6_Cell_Under_T == 3)  //µ¥ÌåÎÂ¶È¹ıµÍ3¼¶
        ||(Error_Group2.Bit.F4_Temp_Over == 3)     //µ¥ÌåÎÂ¶È¹ı¸ß3¼¶
        ||(Error_Group3.Bit.F1_V_CAN_Err)         //ÍâÍøÍ¨Ñ¶¹ÊÕÏ£¨ÓëVCU
        ||(Error_Group3.Bit.F0_Sub_Com_Err)//ÄÚÍøÍ¨Ñ¶¹ÊÕÏ
        ||(Error_Group0.Bit.F0_Fire_Warning==3)       //»ğÔÖÔ¤¾¯3¼¶
//        ||(Error_Group6.Bit.F0_Power_Vol)//Õû³µCANÉÏ±¨
        )
        {
            HighVolPowerOff=1;//
            Error_Group1.Bit.St_DisCHG_Allow=1;//·ÅµçÔÊĞí×´Ì¬Î»²»ÔÊĞí
        }   
    }
    //////////////////////////////¿ì³ä·¢ËÍ¸ßÑ¹ÏÂµçÇëÇó////////////////////
    else if(g_BmsModeFlag == FASTRECHARGING)      
    {      
        if((Error_Group4.Bit.F6_Bat_Over_V>=2)//×ÜµçÑ¹¹ıÑ¹2¼¶¡¢3¼¶
        //||(Error_Group1.Bit.F4_Ch_Over_I == 3)      //³äµç¹ıÁ÷3¼¶
        ||(Error_Group2.Bit.F0_Cell_Over_V>=1)      //µ¥ÌåµçÑ¹¹ıÑ¹1,2,3¼¶
        ||(Error_Group5.Bit.F6_Cell_Under_T == 3)  //µ¥ÌåÎÂ¶È¹ıµÍ3¼¶
        ||(Error_Group2.Bit.F4_Temp_Over == 3)     //µ¥ÌåÎÂ¶È¹ı¸ß3¼¶
        ||(Error_Group1.Bit.F6_Ins_Err == 3)       //¾øÔµµÍ
        ||(Error_Group3.Bit.F1_V_CAN_Err)         //ÍâÍøÍ¨Ñ¶¹ÊÕÏ£¨ÓëÖ±Á÷³äµç»úÍ¨Ñ¶¹ÊÕÏ¡
        ||(Error_Group3.Bit.F0_Sub_Com_Err)//ÄÚÍøÍ¨Ñ¶¹ÊÕÏ
        ||(ACCha_Flag_BST)//µçÁ÷·½ÏòÒì³£¹ÊÕÏ
        ||(Error_Group0.Bit.F0_Fire_Warning==3)       //»ğÔÖÔ¤¾¯3¼¶
 //       ||(Error_Group6.Bit.F0_Power_Vol)//Õû³µCANÉÏ±¨
        ||(Error_Group0.Bit.F2_Ele_Relay_Con ==3)//³äµç²å×ù¹ıÎÂ
        )
        {
            OffState=1;//¿ì³äÇëÇóÏÂµç
            Error_Group1.Bit.St_CHG_Allow=1; //³äµçÔÊĞí×´Ì¬Î»²»ÔÊĞí
        }
    }
    //////////////////////////////ÊÜµç¹­³äµç·¢ËÍ¸ßÑ¹ÏÂµçÇëÇó///////////////
    else if(g_BmsModeFlag == RECHARGING)      
    {      
        if((Error_Group4.Bit.F6_Bat_Over_V>=2)//×ÜµçÑ¹¹ıÑ¹2¼¶¡¢3¼¶
        //||(Error_Group1.Bit.F4_Ch_Over_I == 3)      //³äµç¹ıÁ÷3¼¶
        ||(Error_Group2.Bit.F0_Cell_Over_V>=1)      //µ¥ÌåµçÑ¹¹ıÑ¹1,2,3¼¶
        ||(Error_Group5.Bit.F6_Cell_Under_T == 3)  //µ¥ÌåÎÂ¶È¹ıµÍ3¼¶
        ||(Error_Group2.Bit.F4_Temp_Over == 3)     //µ¥ÌåÎÂ¶È¹ı¸ß3¼¶
        ||(Error_Group3.Bit.F1_V_CAN_Err)         //ÍâÍøÍ¨Ñ¶¹ÊÕÏ£¨ÓëÊÜµç¹­Í¨Ñ¶¹ÊÕÏ¡
        ||(Error_Group3.Bit.F0_Sub_Com_Err)//ÄÚÍøÍ¨Ñ¶¹ÊÕÏ
        ||(ACCha_Flag_BST)//µçÁ÷·½ÏòÒì³£¹ÊÕÏ
        ||(Error_Group0.Bit.F0_Fire_Warning==3)       //»ğÔÖÔ¤¾¯3¼¶
//        ||(Error_Group6.Bit.F0_Power_Vol)//Õû³µCANÉÏ±¨

        )
        {
            PantographOff=1;//ÊÜµç¹­ÇëÇóÏÂµç
            Error_Group1.Bit.St_CHG_Allow=1; //³äµçÔÊĞí×´Ì¬Î»²»ÔÊĞí
        }
    }
}
//******************************************************************************
//* Function name:   BMSProtect
//* Description:     BMS×îºóµÄ±£»¤,²»ÊÜ×´Ì¬»ú¿ØÖÆ£¬Ç¿ÖÆ¶Ï¸ßÑ¹
//* EntryParameter : None
//* ReturnValue    : BoolÀàĞÍ 0-ÎŞ£»1-ÓĞ
//******************************************************************************
void BMSProtect(void)//ÓĞ·çÏÕ£¬Èç¹ûOffStateÅĞ¶ÏÊ§°Ü,×îºÃÓÃÊµ¼ÊµçÑ¹±È½Ï********** 
{
    static unsigned char LCDelaytime=0;
    static unsigned char HCDelaytime=0;
    static unsigned char HTDelaytime=0;
    static unsigned char HBaDelaytime=0;
    if(g_BmsModeFlag == DISCHARGING)
    {      
        if((g_lowestCellVoltage<=LOWEST_CELL_VOL)&&(g_lowestCellVoltage!=0))
        {
            LCDelaytime++;
            if(LCDelaytime>20)
            {
                LCDelaytime=27;
                TurnOffAllRelay();
            }
        } 
        else
        {
            LCDelaytime=0;
        }
    }
    if((g_BmsModeFlag == RECHARGING)||(g_BmsModeFlag == FASTRECHARGING))
    {      
        if(g_highestCellVoltage>=HIGHEST_CELL_VOL )//10s
        {
            HCDelaytime++;
            if(HCDelaytime>30)
            {
                HCDelaytime=37;
                TurnOffAllRelay();
            }
        } 
        else
        {
            HCDelaytime=0;
        }
        if(g_systemVoltage>=HIGHEST_BATT_VOL )//
        {
            HBaDelaytime++;
            if(HBaDelaytime>30)
            {
                HBaDelaytime=37;
                TurnOffAllRelay();
            }
        } 
        else
        {
            HBaDelaytime=0;
        }
    }
    if(g_highestTemperature>(HIGHEST_TEM+40) )//10s
    {
        HTDelaytime++;
        if(HTDelaytime>30)
        {
            HTDelaytime=37;
            TurnOffAllRelay();
        }
    } 
    else
    {
        HTDelaytime=0;
    }
}
//******************************************************************************
//* Function name:   FaultLevel
//* Description:     ¹ÊÕÏµÈ¼¶´¦Àí
//* EntryParameter : None
//* ReturnValue    : ÓĞÎŞ¹ÊÕÏ£º0-ÎŞ£»1-ÓĞ
//******************************************************************************
void FaultLevel(void)
{
    if((Error_Group4.Bit.F6_Bat_Over_V == 3)  //×ÜµçÑ¹¹ıÑ¹3¼¶
    ||(Error_Group4.Bit.F4_Bat_Under_V == 3)  //×ÜµçÑ¹Ç·Ñ¹3¼¶
    //||(Error_Group3.Bit.F6_DisChg_Over_I == 3) //·Åµç¹ıÁ÷3¼¶
    //||(Error_Group5.Bit.F0_FeedB_Over_I == 3)   //»ØÀ¡¹ıÁ÷3¼¶
    ||(Error_Group1.Bit.F4_Ch_Over_I == 3)      //³äµç¹ıÁ÷3¼¶
    ||(Error_Group2.Bit.F0_Cell_Over_V == 3)    //µ¥ÌåµçÑ¹¹ıÑ¹3¼¶
    ||(Error_Group2.Bit.F2_Cell_Under_V == 3)   //µ¥ÌåµçÑ¹Ç·Ñ¹3¼¶
    ||(Error_Group5.Bit.F6_Cell_Under_T == 3)  //µ¥ÌåÎÂ¶È¹ıµÍ3¼¶
    ||(Error_Group2.Bit.F4_Temp_Over == 3)     //µ¥ÌåÎÂ¶È¹ı¸ß3¼¶
    ||(Error_Group1.Bit.F6_Ins_Err == 3)       //¾øÔµµÍ
    ||(Error_Group0.Bit.F0_Fire_Warning==3)       //»ğÔÖÔ¤¾¯3¼¶
    ||(Error_Group3.Bit.F1_V_CAN_Err)//ÍâÍøÍ¨Ñ¶¹ÊÕÏ£¨ÓëÖ±Á÷³äµç»ú¡¢ÓëÊÜµç¹­¡¢VCU)
    ||(Error_Group3.Bit.F0_Sub_Com_Err)//ÄÚÍøÍ¨Ñ¶¹ÊÕÏ
    //||(Error_Group6.Bit.F0_Power_Vol)//Õû³µCANÉÏ±¨
    ||(Error_Group0.Bit.F2_Ele_Relay_Con ==3)//³äµç²å×ù¹ıÎÂ
    ||(ACCha_Flag_BST) //³äµçµçÁ÷·½ÏòÒì³£
    ||(RelayErrorPowerOff)//¼ÌµçÆ÷ÏÂµç¹ÊÕÏ
    )
    {
        status_group3.Bit.Fault_Level = 3;//ÑÏÖØ¹ÊÕÏ£¬½µ¹¦ÂÊÎª0
        status_group4.Bit.St_BMS_System = 1;//BMSÏµÍ³×´Ì¬Î» ¹ÊÕÏ Ö»ÓĞ³öÏÖ3¼¶¹ÊÕÏÉÏ±¨
    } 
    else if((Error_Group4.Bit.F6_Bat_Over_V == 2)//×ÜµçÑ¹¹ıÑ¹2¼¶
    ||(Error_Group4.Bit.F4_Bat_Under_V == 2)     //×ÜµçÑ¹Ç·Ñ¹2¼¶
    ||(Error_Group3.Bit.F6_DisChg_Over_I == 2)   //·Åµç¹ıÁ÷2¼¶
    ||(Error_Group5.Bit.F0_FeedB_Over_I == 2)    //»ØÀ¡¹ıÁ÷2¼¶
    ||(Error_Group1.Bit.F4_Ch_Over_I == 2)       //³äµç¹ıÁ÷2¼¶
    ||(Error_Group2.Bit.F0_Cell_Over_V == 2)     //µ¥ÌåµçÑ¹¹ıÑ¹2¼¶
    ||(Error_Group2.Bit.F2_Cell_Under_V == 2)    //µ¥ÌåµçÑ¹Ç·Ñ¹2¼¶
    ||(Error_Group5.Bit.F4_Cell_Dif_V == 2)      //µ¥ÌåµçÑ¹²»¾ùºâ2¼¶
    ||(Error_Group5.Bit.F2_Cell_Dif_T == 2)      //µ¥ÌåÎÂ¶È²»¾ùºâ2¼¶
    ||(Error_Group5.Bit.F6_Cell_Under_T == 2)    //µ¥ÌåÎÂ¶È¹ıµÍ2¼¶
    ||(Error_Group2.Bit.F4_Temp_Over == 2)       //µ¥ÌåÎÂ¶È¹ı¸ß2¼¶
    ||(Error_Group4.Bit.F0_SOC_Low == 2)         //SOCµÍ2¼¶
    ||(Error_Group1.Bit.F6_Ins_Err == 2)        //¾øÔµµÍ
    ||(Error_Group0.Bit.F0_Fire_Warning==2)       //»ğÔÖÔ¤¾¯2¼¶
    //||(Error_Group6.Bit.F0_Power_Vol)//Õû³µCANÉÏ±¨
    ||(Error_Group0.Bit.F2_Ele_Relay_Con ==2)//³äµç²å×ù¹ıÎÂ
    )
    {
        status_group3.Bit.Fault_Level = 2;//ÇáÎ¢£¬½µ¹¦ÂÊÖÁ50%
    } 
    else if((Error_Group4.Bit.F6_Bat_Over_V) //×ÜµçÑ¹¹ıÑ¹1¼¶
    ||(Error_Group4.Bit.F4_Bat_Under_V)      //×ÜµçÑ¹Ç·Ñ¹1¼¶
    ||(Error_Group3.Bit.F6_DisChg_Over_I)    //·Åµç¹ıÁ÷1¼¶
    ||(Error_Group5.Bit.F0_FeedB_Over_I)     //»ØÀ¡¹ıÁ÷1¼¶
    ||(Error_Group1.Bit.F4_Ch_Over_I)        //³äµç¹ıÁ÷1¼¶
    ||(Error_Group2.Bit.F0_Cell_Over_V)      //µ¥ÌåµçÑ¹¹ıÑ¹1¼¶
    ||(Error_Group2.Bit.F2_Cell_Under_V)     //µ¥ÌåµçÑ¹Ç·Ñ¹1¼¶
    ||(Error_Group5.Bit.F2_Cell_Dif_T)     //µ¥ÌåÎÂ¶È²»¾ùºâ1¼¶
    ||(Error_Group5.Bit.F6_Cell_Under_T)   //µ¥ÌåÎÂ¶È¹ıµÍ1¼¶
    ||(Error_Group2.Bit.F4_Temp_Over)      //µ¥ÌåÎÂ¶È¹ı¸ß1¼¶
    ||(Error_Group4.Bit.F0_SOC_Low)        //SOCµÍ1¼¶
    ||(Error_Group1.Bit.F6_Ins_Err)         //¾øÔµµÍ
    ||((g_caution_Flag_3 &0x02)!=0) //to PC   //µçÁ÷´«¸ĞÆ÷¹ÊÕÏ
    ||(Error_Group0.Bit.F0_Fire_Warning)       //»ğÔÖÔ¤¾¯2¼¶
    ||(Error_Group6.Bit.F0_Power_Vol)//Õû³µCANÉÏ±¨
    )
    {
        status_group3.Bit.Fault_Level = 1;//Ò»°ã
    } 
    else 
    {
        status_group3.Bit.Fault_Level = 0;//ÎŞ¹ÊÕÏ
    }
}
//******************************************************************************
//* Function name:    FaultProcess
//* Description:      ¼ì²â¸÷ÖÖ¹ÊÕÏ: ÆäÖĞÈı¼¶¹ÊÕÏÎª×îÑÏÖØ¹ÊÕÏ
//* EntryParameter : None
//* ReturnValue    : ÓĞÎŞ¹ÊÕÏ£º0-ÎŞ£»1-ÓĞ  
//******************************************************************************
unsigned char TaskFaultProcess(void) 
{
    unsigned char lever = 0;
    //unsigned char i,j,k=0;
    static unsigned char caution1=0;
    static unsigned char caution2=0;
    static unsigned char caution3=0;
    static unsigned char caution4=0;
   
    if((stateCode==30)||(stateCode==110)||(stateCode==170))//
    {
        errorSystemVoltageOV();//×ÜµçÑ¹¹ıÑ¹
        errorChargeOC();   //³äµç¹ıÁ÷ 
        if(stateCode==30) 
        {
            errorSystemVoltageUV();//×ÜµçÑ¹Ç·Ñ¹
            errorDischargeOC();//·Åµç¹ıÁ÷
        }
        if(stateCode==170)
        {
            errorLowIsolation();//¾øÔµ¹ıµÍ
            CHG_SocketOT(); //³äµç²å×ù¹ıÎÂ 
        }
    }
    if((g_BmsModeFlag == FASTRECHARGING)||(g_BmsModeFlag == RECHARGING))
    {
        DCChangerComError();//Ö±Á÷³äµçÍ¨ĞÅ¹ÊÕÏ
        Charge_Check();//³äµçµçÁ÷Õı¸º¼ì²â
    }
    
    if(g_BmsModeFlag == RECHARGING) //Âı³äÄ£Ê½
    {
        ACChangerComError();
    }
    else if(g_BmsModeFlag == DISCHARGING)//ĞĞ³µÄ£Ê½ 
    {
        errorCellVoltageUV();//µ¥ÌåµçÑ¹¹ıµÍ 
        errorSOCLow();//SOCµÍ
        VCUComError();//ÓëVCUÍ¨Ñ¶¹ÊÕÏ
    }                        
    innerCommOT3();//ÄÚ²¿Í¨ĞÅ¹ÊÕÏ 
    errorCellVoltageOV(); //µ¥ÌåµçÑ¹¹ı¸ß 
    errorCurrSensorIniatial(); //µçÁ÷´«¸ĞÆ÷¹ÊÕÏ
    errorCellUnbalance();//µ¥ÌåµçÑ¹²»¾ùºâ
    errorTemUnbalance();//µ¥ÌåÎÂ¶È²»¾ùºâ
    errorCellTemperatureOT();//µ¥ÌåÎÂ¶È¹ı¸ß
    errorCellTemperatureUT();//µ¥ÌåÎÂ¶È¹ıµÍ
    Fire_Warning();//»ğÔÖÔ¤¾¯
    PowerSupplyError();//¹©µçµçÔ´Òì³£
    
    
    FaultLevel(); //¹ÊÕÏµÈ¼¶ÅĞ¶Ï

    BMSProtect();////////BMS×îºó¶Ï¼ÌµçÆ÷

    //*****************************************************************************************
    //******************************************************************************************
    ////////////µ±¹ÊÕÏÓëÖ®Ç°Ïà±È£¬Ã»ÓĞ±ä»¯Ê±£¬²»½øĞĞ´æ´¢£¬Ö»ÓĞ1·ÖÖÓ²Å´æ´¢£»Èç¹ûÓĞ¸üĞÂ£¬²Å½øĞĞ´æ´¢
    if((caution1^g_caution_Flag_1)||(caution2^g_caution_Flag_2)||(caution3^g_caution_Flag_3)||(caution4^g_caution_Flag_4))
        StoreSysVariable();//±£´æSOCÖµºÍ¹ÊÕÏĞÅÏ¢
    caution1 =  g_caution_Flag_1;
    caution2 =  g_caution_Flag_2;
    caution3 =  g_caution_Flag_3;
    caution4 =  g_caution_Flag_4;
    //////////////////////////////////////////////////////////////////
    ////Caution_Flag_4£ºBit 0£º¸ßÑ¹Ä¸ÏßÁ¬½Ó¹ÊÕÏ£»Bit 1£ºÑÌÎí¼ì²â¹ÊÕÏ£»    
    ///////////////////  
    g_storageSysVariable[PARA_ERROR_LEVER] = lever;
    g_storageSysVariable[CAUTION_FLAG_1] = g_caution_Flag_1;	
    g_storageSysVariable[CAUTION_FLAG_2] = g_caution_Flag_2;	
    g_storageSysVariable[CAUTION_FLAG_3] = g_caution_Flag_3;	
    g_storageSysVariable[CAUTION_FLAG_4] = g_caution_Flag_4;	
  
	  return g_storageSysVariable[PARA_ERROR_LEVER];   
}
//******************************************************************************
//* Function name:   TurnOffAllRelay
//* Description:     BMS×îºóµÄ±£»¤,²»ÊÜ×´Ì¬»ú¿ØÖÆ£¬Ç¿ÖÆ¶Ï¸ßÑ¹
//* EntryParameter : ¶Ï¿ªËùÓĞ¼ÌµçÆ÷
//* ReturnValue    : void
//******************************************************************************
int TurnOffAllRelay(void)//
{
    openNegRelay();
    delay(25000); //19ms
    openPosRelay();
    delay(25000); //19ms
    TurnOff_INA2K();//¿ì³ä¼ÌµçÆ÷
    delay(25000); //19ms
    TurnOff_INA1K();//¿ì³ä¼ÌµçÆ÷
    delay(25000); //19ms
    TurnOff_INBK();//¿ì³ä¼ÌµçÆ÷
    delay(25000); //19ms
    TurnOff_INHK();//¿ì³ä¼ÌµçÆ÷
    delay(25000); //19ms
    Error_Group6.Bit.F3_BMS_Protect = 1;//Õû³µCAN¸³Öµ BMS¼«ÏŞ¹ÊÕÏ×ÔÎÒ±£»¤
}
//********************************************************************************************
//***********************************the end*************************************************
//********************************************************************************************
//********************************************************************************************
