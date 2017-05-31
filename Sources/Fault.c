//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : �����ֽ�6123
//* File Name          : Fault.c
//* Author             : SLX
//* Version            : V1.0.0
//* Start Date         : 2016.11.30
//* Description        : ���ļ� ��ϵͳ�ĸ��������ж�
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h"
//#include "Fault.h" 
///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 

//***********************************************************************************
//***********************************************************************************
unsigned char  g_caution_Flag_1 =0;//BMS����״̬ FOR ��λ��
unsigned char  g_caution_Flag_2 =0;
unsigned char  g_caution_Flag_3 =0;
unsigned char  g_caution_Flag_4 =0;
unsigned int g_errorCounter;
unsigned char ACCha_Flag_BST=0;
unsigned char ACCOverTime=0;//��������ͨ�Ź���
unsigned char VCUOverTime=0;//��VCUͨѶ����
unsigned char SBMSOverTime=0;//����һ����SBMSͨѶ����
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

unsigned char test1=0;//


//******************************************************************************
//* Function name:    ErrorToCarPC
//* Description:     ��ͬ���ϵĴ�����ʽ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
/*void ErrorToCarPC(unsigned char Err1,unsigned char Err2,unsigned char Err3,unsigned char Err4,unsigned char *CarError,
                  unsigned char *PCErrorLevel1,unsigned char *PCErrorLevel2, unsigned char *PCErrorLevel3)//
{
    if(Err1==1)//���1������ 
    {
        *CarError=1;
        *PCErrorLevel1 |= 0x04;//to PC
    } 
    else//û��1������
    {           
        if(Err1==1)//����ж�������
        {
            *CarError=2;
            *PCErrorLevel2=1;//to PC
        } 
        else//���û�ж�������
        {
            *CarError=0;
            *PCErrorLevel2=0;//to PC                      
            if(Err1==1)//�������������
            {
                *CarError=3;
                *PCErrorLevel3=1;//to PC 
            } 
            else//��������޹���
            {
                *CarError=0;
                *PCErrorLevel3=0;//to PC 
                if(Err1==1)//4������
                    *CarError = 4;                
                else  //��4��
                    *CarError = 0;                              
            }
        }
    }
} */
//******************************************************************************
//* Function name:    errorSystemVoltageOV
//* Description:      ��ذ��ܵ�ѹ��ѹ���� 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorSystemVoltageOV(void) 
{
    unsigned char Error[4]={0};
    unsigned char i;
    unsigned char Level=0; 
    //��Ϊ���벻һ�������Ե��õ�ģ���ڲ�ͬģʽ�����벻ͬ        
   
    if(stateCode == 30)//�г�
        Level=TotalVoltageOverVoltage_custom(g_systemVoltage,DISCHARGING,g_highVoltageV3);
           
    else if(stateCode == 170)//���ģʽ
        Level=TotalVoltageOverVoltage_custom(g_systemVoltage,FASTRECHARGING,g_highVoltageV5);
    
    else if(stateCode == 110) //����
        Level=TotalVoltageOverVoltage_custom(g_systemVoltage,RECHARGING,g_highVoltageV6);
    ///////////////////�ϱ����ϵȼ���/////////////////////////   
    for(i=1;i<4;i++) 
       if(i==Level) 
           Error[i]=1;
       
    Error_Group4.Bit.F6_Bat_Over_V = Level;//����CAN��ֵ   
    //1������
    Can554Byte2.Bit.F2_systemOV1=Error[1];//�ڲ�CAN��ֵ
    //CutChaCurt70.Bit.F0_Battery_Over_Voltage3=Error[1];
    //CutDCChaCurt70.Bit.F0_Battery_Over_Voltage3=Error[1];
    //CutACChaCurt70.Bit.F0_Battery_Over_Voltage3=Error[1]; 
    
    //2������
    Can554Byte0.Bit.F2_systemOV2=Error[2] ;
    CutChaCurt50.Bit.F0_Battery_Over_Voltage2=Error[2] ;
    CutDCChaCurt0.Bit.F0_Battery_Over_Voltage21=Error[2] ;  
    CutACChaCurt0.Bit.F0_Battery_Over_Voltage21=Error[2];
    
    //3������
    if(Error[3]==1)
        g_caution_Flag_1 |= 0x04;
    else 
    {
        if(stateCode == 30)
            g_caution_Flag_1 &= 0xfb;    
    }
    CutChaCurt0.Bit.F0_Battery_Over_Voltage1=Error[3] ;
    CutDCChaCurt0.Bit.F0_Battery_Over_Voltage21=Error[3] ;
    CutACChaCurt0.Bit.F0_Battery_Over_Voltage21=Error[3]; 
}
//******************************************************************************
//* Function name:    errorSystemVoltageUV
//* Description:      ��ذ��ܵ�ѹǷѹ���� 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorSystemVoltageUV(void) //�ָ�,�ϱ�
{   
    unsigned char i;
    unsigned char Error[4]={0};
    unsigned char Level=0; 
    ///////////////////�ϱ����ϵȼ���/////////////////////////
    
    Level= TotalVoltageUnderVoltage_custom(g_systemVoltage,g_highVoltageV3); 
    
    for(i=1;i<4;i++) 
       if(i==Level) 
           Error[i]=1;
       
    Error_Group4.Bit.F4_Bat_Under_V = Level;//����CAN��ֵ
    
    //1�����ϴ���
    Can554Byte2.Bit.F0_systemUV1=Error[1]; //to PC
    //CutDisCurt70.Bit.F0_Battery_Under_Voltage3=Error[1];
    
    
    //2������
    Can554Byte0.Bit.F0_systemUV2=Error[2];//to PC
    CutDisCurt50.Bit.F0_Battery_Under_Voltage2=Error[2];
    
    //3������ 
    if(Error[3] == 1) 
    {
        g_caution_Flag_1 |=0x01;
        status_group1.Bit.St_BMS =2;//BMS״̬��ѹ�Ͽ�

    }
    CutDisCurt0.Bit.F0_Battery_Under_Voltage1=Error[3];
    
}

//******************************************************************************
//* Function name:    errorDischargeOC
//* Description:      �ŵ��������  һ��
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorDischargeOC(void) //�ָ�;�ϱ������� 
{
    unsigned char Error[4]={0};
    unsigned char i;
    unsigned char Level=0; 
    float curtValue=0;

    curtValue = BiggestDischargeCurt;
    
    ////////////////////////�ϱ����ϵȼ�////////////////////////
    Level=DisChargeOverCurrent_custom(g_systemCurrent,curtValue);  
    for(i=1;i<4;i++) 
        if(i==Level) 
            Error[i]=1;
     
    Error_Group3.Bit.F6_DisChg_Over_I=Level;//����CAN��ֵ         
    //1�����ϴ���
    Can554Byte3.Bit.F0_DisChaOCurt1=Error[1];
    //CutDisCurt70.Bit.F1_DisCharge_Over_Current3=Error[1];

    //2�����ϴ���
    Can554Byte1.Bit.F0_DisChaOCurt2=Error[2];//to PC
    //CutDisCurt50.Bit.F1_DisCharge_Over_Current2=Error[2];
     
    //3�����ϴ���
    /*if(Error[3]) 
    {
        g_caution_Flag_2 |= 0x01;//to PC
        //status_group2.Bit.BMS_PowerOff_Req = 2;//BMS��ѹ�µ�����
    }
     */
    //CutDisCurt0.Bit.F2_DisCharge_Over_Current1=Error[3];
}
//******************************************************************************
//* Function name:    errorChargeOC
//* Description:      ���������Ϻͻ�����������ͬһģ�� 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorChargeOC(void)
{
    unsigned char Error[4]={0};
    unsigned char i;
    unsigned char Level=0; 
    float curtValue=0;
    
    if(g_BmsModeFlag == DISCHARGING)//�ŵ�ģʽ
    {
        curtValue=BiggestFeedbackCurt; //��������Ϊ��ֵ
    }
    else if((g_BmsModeFlag == FASTRECHARGING)||(g_BmsModeFlag == RECHARGING)) //���ģʽ
    {
        curtValue=m_askcurrent;
    } 
    ///////////////////�ϱ����ϵȼ���/////////////////////////    
    Level=ChargeOverCurrent_custom((-g_systemCurrent),curtValue);    
    for(i=1;i<4;i++) 
        if(i==Level) 
            Error[i]=1;         
    
    if(g_BmsModeFlag == DISCHARGING)
        Error_Group5.Bit.F0_FeedB_Over_I = Level;//����CAN��ֵ,˲ʱ����
    else
    {
        Error_Group1.Bit.F4_Ch_Over_I = Level;//����CAN��ֵ,��繦��
        
    }    
    if(Level>=1)
        Error_Group6.Bit.F5_Chg_C_Over = 1;//����CAN��ֵ,���������ޱ���
    else
        Error_Group6.Bit.F5_Chg_C_Over = 0;//����CAN��ֵ,���������ޱ���
    //1�����ϴ���
    Can554Byte2.Bit.F7_ChangerOCurt1=Error[1];//to PC
    //CutChaCurt70.Bit.F1_Charge_Over_Current3=Error[1];
    //CutDCChaCurt70.Bit.F1_Charge_Over_Current3=Error[1];
    //CutACChaCurt70.Bit.F1_Charge_Over_Current3=Error[1];
    
    //2�����ϴ���
    Can554Byte0.Bit.F7_ChangerOCurt2=Error[2];//to PC
    //CutChaCurt50.Bit.F1_Charge_Over_Current2=Error[2];
    //CutDCChaCurt50.Bit.F0_Charge_Over_Current2=Error[2];
    //CutACChaCurt50.Bit.F0_Charge_Over_Current2=Error[2];
    
    //3�����ϴ���
    //if(Error[3])
    //{
     //   g_caution_Flag_1 |=0x80;//to PC
        //if(g_BmsModeFlag == DISCHARGING)
            //status_group2.Bit.BMS_PowerOff_Req = 2;//BMS��ѹ�µ�����
   // }

    //CutChaCurt0.Bit.F2_Charge_Over_Current1=Error[3];
    //CutDCChaCurt0.Bit.F3_Charge_Over_Current1=Error[3];
    //CutACChaCurt0.Bit.F3_Charge_Over_Current1=Error[3];
}
//******************************************************************************
//* Function name:    errorCellVoltageOV
//* Description:      �����ѹ����,1�����ϲ��ָ�,2,3,4�����Ͽɻָ�,ֱ������,��ֱ���ϱ�2������,���Ե�1Ҳ���Ե�3,���1��ֱ�Ӳ��ָ�  
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorCellVoltageOV(void)
{

    unsigned char Error[4]={0};
    unsigned char i;
    unsigned char Level=0; 
    //�ϱ����ϵȼ��ȼ��� 
    Level=CellOverVoltage_custom(g_highestCellVoltage,g_BmsModeFlag);      
    for(i=1;i<4;i++) 
       if(i==Level) 
           Error[i]=1;
    
    Error_Group2.Bit.F0_Cell_Over_V=Level;//����CAN��ֵ
    //1������ 
    Can554Byte2.Bit.F3_cellOV1=Error[1];
    //CutChaCurt70.Bit.F2_Cell_Over_Voltage3=Error[1];
    CutACChaCurt0.Bit.F4_Cell_Over_Voltage321=Error[1];
    CutDCChaCurt0.Bit.F4_Cell_Over_Voltage321=Error[1];
    
    //2������     
    Can554Byte0.Bit.F3_cellOV2=Error[2];//to PC
    CutChaCurt50.Bit.F2_Cell_Over_Voltage2=Error[2];
    CutDCChaCurt0.Bit.F4_Cell_Over_Voltage321=Error[2];
    CutACChaCurt0.Bit.F4_Cell_Over_Voltage321=Error[2]; 
    
    //3������
    if(Error[3]) 
    {
        g_caution_Flag_1 |= 0x08;//to PC
        if(g_BmsModeFlag == DISCHARGING)
            status_group1.Bit.St_BMS =2;//BMS״̬��ѹ�Ͽ�
    }
    CutDisCurt0.Bit.F10_Cell_Over_Voltage1= Error[3];
    CutChaCurt0.Bit.F3_Cell_Over_Voltage1=Error[3];
    CutACChaCurt0.Bit.F4_Cell_Over_Voltage321=Error[3];
    CutDCChaCurt0.Bit.F4_Cell_Over_Voltage321=Error[3];
        
}
//******************************************************************************
//* Function name:   errorCellVoltageUV
//* Description:     �����ѹǷѹ����  
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorCellVoltageUV(void)//�ϱ�������,��粻�ϱ�,�ָ�
{
    unsigned char i;
    unsigned char Error[4]={0};
    unsigned char Level=0; 

    Level=CellUnderVoltage_custom(g_lowestCellVoltage);
    for(i=1;i<4;i++)
        if(i==Level)
            Error[i]=1;
      
    Error_Group2.Bit.F2_Cell_Under_V=Level;//����CAN��ֵ
    //1�����ϴ���
    Can554Byte2.Bit.F1_cellUV1=Error[1];//to PC
    //CutDisCurt70.Bit.F2_Cell_Under_Voltage3=Error[1];

    //2�����ϴ���
    Can554Byte0.Bit.F1_cellUV2=Error[2];//to PC
    CutDisCurt50.Bit.F2_Cell_Under_Voltage2=Error[2];
     
    //3�����ϴ���
    if(Error[3])
    {
        g_caution_Flag_1 |= 0x02;//to PC
            status_group1.Bit.St_BMS =2;//BMS״̬��ѹ�Ͽ�
    }
    CutDisCurt0.Bit.F3_Cell_Under_Voltage1=Error[3];
    CutChaCurt0.Bit.F10_Cell_Under_Voltage1=Error[3];

}
//******************************************************************************
//* Function name:    errorCurrSensor
//* Description:      �������������� 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
Bool errorCurrSensorIniatial(void) //�ϵ�ǰ���2��
{
    unsigned char i;
    unsigned char Error[4]={0};
    unsigned char Level=0; 

    Level=CurrentSensorFault_custom(g_systemCurrent);
    for(i=1;i<4;i++)
        if(i==Level)
            Error[i]=1;
        
    if(Error[1])
        g_caution_Flag_3 |=0x02; //to PC      //��������������
    //hardware_error2.Bit.F7_I_Ga_Err = 1;
    /*if(g_BmsModeFlag == DISCHARGING) 
    {
        CutDisCurt0.Bit.F4_I_Sener_Err = 1;
        CutChaCurt0.Bit.F4_I_Sener_Err = 1;
        status_group2.Bit.BMS_PowerOff_Req = 2;//BMS��ѹ�µ����� 
    }
    else if(g_BmsModeFlag == FASTRECHARGING)
        CutDCChaCurt0.Bit.F5_I_Sener_Err =1;
    else if(g_BmsModeFlag == RECHARGING)
        CutACChaCurt0.Bit.F5_I_Sener_Err =1;
               
    }*/    
    return 0;  
}
//******************************************************************************
//* Function name:    errorCurrSensor
//* Description:      �¶ȴ��������� 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************

//******************************************************************************
//* Function name:    errorCellUnbalance
//* Description:    �����ѹ��������� 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorCellUnbalance(void)//
{
    unsigned char Error[4]={0};
    unsigned char i;
    unsigned char Level=0;

    //�ϱ����ϵȼ�     
    Level=CellVolUnbalance_custom(g_highestCellVoltage,g_lowestCellVoltage);
    for(i=1;i<4;i++) 
       if(i==Level) 
           Error[i]=1;
       
    Error_Group5.Bit.F4_Cell_Dif_V=Level;//����CAN��ֵ 
    //2�����ϴ���
    Can554Byte1.Bit.F2_CellUnbalance2=Error[2];//to PC
    CutDisCurt50.Bit.F3_CellUnbalance2=Error[2];
    CutChaCurt50.Bit.F6_CellUnbalance2=Error[2];
    CutDCChaCurt50.Bit.F6_CellUnbalance2=Error[2];
    CutACChaCurt50.Bit.F5_CellUnbalance2=Error[2];

     
}
//******************************************************************************
//* Function name:    errorTemUnbalance
//* Description:    �����¶Ȳ�������� 
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
    
    //�ϱ����ϵȼ�
    Level=CellTempUnbalance_custom(g_highestTemperature,g_lowestTemperature);
    for(i=1;i<4;i++)
        if(i==Level)
            Error[i]=1;
        
    Error_Group5.Bit.F2_Cell_Dif_T=Level;//����CAN��ֵ 
    //1�����ϴ���
    Can554Byte3.Bit.F3_tempUnbalance1=Error[1];//to PC
    //CutDisCurt70.Bit.F3_tempUnbalance3=Error[3];
    //CutChaCurt70.Bit.F3_tempUnbalance3=Error[3];
    //CutDCChaCurt70.Bit.F2_tempUnbalance3=Error[3];
    //CutACChaCurt70.Bit.F2_tempUnbalance3=Error[3];
    
    //2�����ϴ���
    Can554Byte1.Bit.F3_tempUnbalance2=Error[2];//to PC
    CutDisCurt50.Bit.F4_tempUnbalance2=Error[2];
    CutChaCurt50.Bit.F3_tempUnbalance2=Error[2];
    CutDCChaCurt50.Bit.F1_tempUnbalance2=Error[2];
    CutACChaCurt50.Bit.F1_tempUnbalance2=Error[2];
    
    
}
//******************************************************************************
//* Function name:   errorCellTemperatureUT
//* Description:     �����¶ȹ��͹��� 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorCellTemperatureUT(void)//
{
    unsigned char i;
    unsigned char Error[4]={0}; 
    unsigned char Level=0;
    float t;
    //t= (float)g_lowestTemperature;
    
    //�ϱ����ϵȼ�
    Level=BatteryTemperatureLow_custom(g_lowestTemperature);
    for(i=1;i<4;i++) 
       if(i==Level) 
           Error[i]=1;
    
    Error_Group5.Bit.F6_Cell_Under_T=Level;//����CAN��ֵ
    //1�����ϴ���
    Can554Byte2.Bit.F6_cellUT1=Error[1];
    
    //2�����ϴ���
    Can554Byte0.Bit.F6_cellUT2=Error[2];
    
     //3�����ϴ���
    if(Error[3]) 
    {
        g_caution_Flag_1 |= 0x40;//to PC
        if(g_BmsModeFlag == DISCHARGING)
            status_group1.Bit.St_BMS =2;//BMS״̬��ѹ�Ͽ�
    }
    CutDisCurt0.Bit.F6_Under_Temp1=Error[3];
    CutChaCurt0.Bit.F6_Under_Temp1=Error[3];
    CutDCChaCurt0.Bit.F7_Under_Temp1=Error[3];
    CutACChaCurt0.Bit.F7_Under_Temp1=Error[3];
     
} 
//******************************************************************************
//* Function name:    errorCellTemperatureOT
//* Description:      �����¶ȹ��߹���  һ��
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorCellTemperatureOT(void)//
{
    unsigned char i;
    unsigned char Error[4]={0}; 
    unsigned char Level=0; 
    float t;
    //t= (float)g_highestTemperature;
    
    //�ϱ����ϵȼ�
    Level=BatteryTemperatureHigh_custom(g_highestTemperature);
    
    for(i=1;i<4;i++) 
       if(i==Level) 
           Error[i]=1;    
    
    Error_Group2.Bit.F4_Temp_Over=Level;//����CAN��ֵ
    //1�����ϴ���
    Can554Byte2.Bit.F5_cellOT1=Error[1];
    //CutDisCurt70.Bit.F4_Over_Temp3=Error[1];
    //CutChaCurt70.Bit.F4_Over_Temp3=Error[1];
    //CutDCChaCurt70.Bit.F3_Over_Temp3=Error[1];
    //CutACChaCurt70.Bit.F3_Over_Temp3=Error[1];
    
    
    //2�����ϴ���
    Can554Byte0.Bit.F5_cellOT2=Error[2];//to PC
    CutDisCurt50.Bit.F5_Over_Temp2=Error[2];
    CutChaCurt50.Bit.F4_Over_Temp2=Error[2];
    CutDCChaCurt50.Bit.F2_Over_Temp2=Error[2];
    CutACChaCurt50.Bit.F2_Over_Temp2=Error[2];
    
    //3�����ϴ���
    if(Error[3])
    {  
        g_caution_Flag_1 |= 0x20;//to PC
        if(g_BmsModeFlag == DISCHARGING)
            status_group1.Bit.St_BMS =2;//BMS״̬��ѹ�Ͽ�
    }
    CutDisCurt0.Bit.F5_Over_Temp1=Error[3];
    CutChaCurt0.Bit.F5_Over_Temp1=Error[3];
    CutDCChaCurt0.Bit.F6_Over_Temp1=Error[3];
    CutACChaCurt0.Bit.F6_Over_Temp1=Error[3];
    
}

//******************************************************************************
//* Function name:    errorSOCLow
//* Description:      SOC�͹��� һ��
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorSOCLow(void)//
{
    unsigned char i;
    unsigned char Error[4]={0}; 
    unsigned char Level=0;
    float t;
   
    //�ϱ����ϵȼ�
    Level=socLow_custom(Can_g_socValue);
    for(i=1;i<4;i++) 
       if(i==Level) 
           Error[i]=1;
       
    Error_Group4.Bit.F0_SOC_Low=Level;//����CAN��ֵ 
    //1�����ϴ���
    Can554Byte3.Bit.F1_SOCLow1=Error[1];
    
    //2�����ϴ���
    Can554Byte1.Bit.F1_SOCLow2=Error[2];
    
     
}
//******************************************************************************
//* Function name:    errorLowIsolation
//* Description:      ��Ե������͹��� 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void errorLowIsolation(void) 
{
    unsigned char i;
    unsigned char Error[4]={0};
    unsigned char Level=0; 
    float LowInsolation;
    if(Rn_Vpn_Value<=Rp_Vpn_Value)
        LowInsolation = Rn_Vpn_Value;
    else
        LowInsolation = Rp_Vpn_Value;
    
    Level=IsolationLow_custom(LowInsolation);
    for(i=1;i<4;i++)
      if(i==Level)
        Error[i]=1;
      
    Error_Group1.Bit.F6_Ins_Err=Level;//����CAN��ֵ
    if(Level>=1)
        Error_Group6.Bit.F7_Chg_Ins_Low=1;//����Ե���ͱ���
            
    //1�����ϴ���
    Can554Byte2.Bit.F4_insulationLow1=Error[1];//to PC
    //CutDCChaCurt70.Bit.F4_Low_Isolation3=Error[1]; //û��70���ˣ�ֻ�ϱ������� 
    //CutACChaCurt70.Bit.F4_Low_Isolation3=Error[1];

//    CutACChaCurt0.Bit.F8_Low_Isolation1=Error[2];//���䲻���
    
    //2�����ϴ���
    Can554Byte0.Bit.F4_insulationLow2=Error[2];//to PC
    CutDCChaCurt50.Bit.F3_Low_Isolation2=Error[2];
//    CutACChaCurt50.Bit.F3_Low_Isolation2=Error[2];
   
    //3�����ϴ���
    if(Error[3])
        g_caution_Flag_1 |= 0x10;//to PC
     CutDCChaCurt0.Bit.F8_Low_Isolation1=Error[3];

     
}
//******************************************************************************
//* Function name:    DCChangerComError
//* Description:      ��ֱ������ͨ�Ź��ϣ�������ʱ���ϣ�
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void DCChangerComError(void)
{
    if(OverTimeState==1)
    {
        CutDCChaCurt0.Bit.F1_Communication_With_Charger=1;//���϶���
        g_caution_Flag_2 |=0x80; //for �ڲ�CAN
        Error_Group3.Bit.F1_V_CAN_Err =1; //����CAN
    }
}
//******************************************************************************
//* Function name:    ACChangerComError
//* Description:      ���ܵ繭����ͨ�Ź��ϣ�������ʱ���ϣ�
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void ACChangerComError(void)
{
    ACCOverTime++;
    if(ACCOverTime>=30)//30sû�н��յ���������
    {
        CutACChaCurt0.Bit.F1_Communication_With_Charger=1;
        g_caution_Flag_2 |=0x10; //for �ڲ�CAN
        Error_Group3.Bit.F1_V_CAN_Err =1; //����CAN
    }
    
}
//******************************************************************************
//* Function name:    VCUComError
//* Description:      ������ͨ�Ź���
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void VCUComError(void)
{
    VCUOverTime++;
    if(VCUOverTime>=30)//30sû�н��յ���������
    {
        CutDisCurt0.Bit.F7_VCU_Communiction=1;
        CutChaCurt0.Bit.F7_VCU_Communiction=1;
        //g_caution_Flag_2 |=0x10; //for �ڲ�CAN
        Error_Group3.Bit.F1_V_CAN_Err =1; //����CAN
            status_group1.Bit.St_BMS =2;//BMS״̬��ѹ�Ͽ�
    }
}
//******************************************************************************
//* Function name:    innerCommOT3
//* Description:      �ڲ�ͨѶ����  ����
//* EntryParameter : None
//* ReturnValue    : Bool���� 0-�ޣ�1-��
//******************************************************************************
void innerCommOT3(void) 
{ 
    SBMSOverTime++;
    if(((g_caution_Flag_3 & 0x01)!=0)||(SBMSOverTime>=30)) //BMU�ڲ�ͨѶ���ϻ���SBMS����
    {
        status_group1.Bit.St_BMS =2;//BMS״̬��ѹ�Ͽ�
        CutDisCurt0.Bit.F1_Inner_Communiction=1;
        CutChaCurt0.Bit.F1_Inner_Communiction=1;
        CutDCChaCurt0.Bit.F2_Inner_Communiction=1;
        CutACChaCurt0.Bit.F2_Inner_Communiction=1;
        
        Error_Group3.Bit.F0_Sub_Com_Err=1;//����CAN��ֵ 
    } 
}

//******************************************************************************
//* Function name:    Charge_Check
//* Description:      ����������� 
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
            if(Check_Num>=250) //���������100�Σ����㷨Ҫ������µ硣
                Check_Num=101;
        }
    }
}
//******************************************************************************
//* Function name:    CHG_SocketOT
//* Description:      ���������� 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void CHG_SocketOT(void) 
{
    unsigned char i;
    unsigned char Error[4]={0}; 
    unsigned char Level=0;
    float tmax;
    if(DCTem1>=DCTem2)
        tmax=DCTem1;
    else
        tmax=DCTem2;
    
   
    //�ϱ����ϵȼ�
    if(tmax>=0) //�������0
        Level=ChargeSocketOverTemp_custom((unsigned char)tmax);
    else 
        Level=0;
    for(i=1;i<4;i++) 
       if(i==Level) 
           Error[i]=1;
       
    Error_Group0.Bit.F2_Ele_Relay_Con=Level;//����CAN��ֵ     

    ///////2�����ϴ���
    
    CutDCChaCurt50.Bit.F5_CHG_Socket2=Error[2];
    
    /////3�����ϴ��� 
    CutDCChaCurt0.Bit.F12_CHG_Socket1=Error[3];
    
}
//******************************************************************************
//* Function name:    Fire_Warning
//* Description:      ����Ԥ�� 
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
            status_group1.Bit.St_BMS =2;//BMS״̬��ѹ�Ͽ�
    }
} 
//******************************************************************************
//* Function name:    PowerSupplyError
//* Description:      �����Դ�쳣
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void PowerSupplyError(void)//
{
    unsigned char i;
    unsigned char Error[4]={0}; 
    unsigned char Level=0;

    //�ϱ����ϵȼ�
    
    Level=Supply24V_custom(PowerVOL);
    for(i=1;i<4;i++) 
       if(i==Level) 
           Error[i]=1;
    
    //1�����ϴ��� �ϱ�����  
    if(Error[1]) 
    {
        //Error_Group6.Bit.F0_Power_Vol=1;//����CAN�ϱ�
        /*CutDisCurt0.Bit.F9_Supply_Power1=1;
        CutChaCurt0.Bit.F9_Supply_Power1=1;
        CutDCChaCurt0.Bit.F11_Supply_Power1=1;
        CutACChaCurt0.Bit.F11_Supply_Power1=1;
        */
    } 
}
//**********************************************************************
//* Function name:   CarFaultDone
//* Description:     �г������й��ϵĴ���
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void CarFaultDone()
{
    //////////////////////////////�г����͸�ѹ�µ�����/////////////////////
    
    if(g_BmsModeFlag == DISCHARGING) //�г����͸�ѹ�µ�����
    {
        if((Error_Group4.Bit.F4_Bat_Under_V ==3)//�ܵ�ѹ��ѹ3��
        //||(Error_Group3.Bit.F6_DisChg_Over_I == 3) //�ŵ����3��
        //||(Error_Group5.Bit.F0_FeedB_Over_I == 3)  //��������3��
        ||(Error_Group2.Bit.F0_Cell_Over_V==3)      //�����ѹ��ѹ3��
        ||(Error_Group2.Bit.F2_Cell_Under_V == 3)   //�����ѹǷѹ3��
        ||(Error_Group5.Bit.F6_Cell_Under_T == 3)  //�����¶ȹ���3��
        ||(Error_Group2.Bit.F4_Temp_Over == 3)     //�����¶ȹ���3��
        ||(Error_Group3.Bit.F1_V_CAN_Err)         //����ͨѶ���ϣ���VCU
        ||(Error_Group3.Bit.F0_Sub_Com_Err)//����ͨѶ����
        ||(Error_Group0.Bit.F0_Fire_Warning==3)       //����Ԥ��3��
//        ||(Error_Group6.Bit.F0_Power_Vol)//����CAN�ϱ�
        )
        {
            HighVolPowerOff=1;//
            Error_Group1.Bit.St_DisCHG_Allow=1;//�ŵ�����״̬λ������
        }   
    }
    //////////////////////////////��䷢�͸�ѹ�µ�����////////////////////
    else if(g_BmsModeFlag == FASTRECHARGING)      
    {      
        if((Error_Group4.Bit.F6_Bat_Over_V>=2)//�ܵ�ѹ��ѹ2����3��
        //||(Error_Group1.Bit.F4_Ch_Over_I == 3)      //������3��
        ||(Error_Group2.Bit.F0_Cell_Over_V>=1)      //�����ѹ��ѹ1,2,3��
        ||(Error_Group5.Bit.F6_Cell_Under_T == 3)  //�����¶ȹ���3��
        ||(Error_Group2.Bit.F4_Temp_Over == 3)     //�����¶ȹ���3��
        ||(Error_Group1.Bit.F6_Ins_Err == 3)       //��Ե��
        ||(Error_Group3.Bit.F1_V_CAN_Err)         //����ͨѶ���ϣ���ֱ������ͨѶ���ϡ
        ||(Error_Group3.Bit.F0_Sub_Com_Err)//����ͨѶ����
        ||(ACCha_Flag_BST)//���������쳣����
        ||(Error_Group0.Bit.F0_Fire_Warning==3)       //����Ԥ��3��
 //       ||(Error_Group6.Bit.F0_Power_Vol)//����CAN�ϱ�
        ||(Error_Group0.Bit.F2_Ele_Relay_Con ==3)//����������
        )
        {
            OffState=1;//��������µ�
            Error_Group1.Bit.St_CHG_Allow=1; //�������״̬λ������
        }
    }
    //////////////////////////////�ܵ繭��緢�͸�ѹ�µ�����///////////////
    else if(g_BmsModeFlag == RECHARGING)      
    {      
        if((Error_Group4.Bit.F6_Bat_Over_V>=2)//�ܵ�ѹ��ѹ2����3��
        //||(Error_Group1.Bit.F4_Ch_Over_I == 3)      //������3��
        ||(Error_Group2.Bit.F0_Cell_Over_V>=1)      //�����ѹ��ѹ1,2,3��
        ||(Error_Group5.Bit.F6_Cell_Under_T == 3)  //�����¶ȹ���3��
        ||(Error_Group2.Bit.F4_Temp_Over == 3)     //�����¶ȹ���3��
        ||(Error_Group3.Bit.F1_V_CAN_Err)         //����ͨѶ���ϣ����ܵ繭ͨѶ���ϡ
        ||(Error_Group3.Bit.F0_Sub_Com_Err)//����ͨѶ����
        ||(ACCha_Flag_BST)//���������쳣����
        ||(Error_Group0.Bit.F0_Fire_Warning==3)       //����Ԥ��3��
//        ||(Error_Group6.Bit.F0_Power_Vol)//����CAN�ϱ�

        )
        {
            PantographOff=1;//�ܵ繭�����µ�
            Error_Group1.Bit.St_CHG_Allow=1; //�������״̬λ������
        }
    }
}
//******************************************************************************
//* Function name:   BMSProtect
//* Description:     BMS���ı���,����״̬�����ƣ�ǿ�ƶϸ�ѹ
//* EntryParameter : None
//* ReturnValue    : Bool���� 0-�ޣ�1-��
//******************************************************************************
void BMSProtect(void)//�з��գ����OffState�ж�ʧ��,�����ʵ�ʵ�ѹ�Ƚ�********** 
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
//* Description:     ���ϵȼ�����
//* EntryParameter : None
//* ReturnValue    : ���޹��ϣ�0-�ޣ�1-��
//******************************************************************************
void FaultLevel(void)
{
    if((Error_Group4.Bit.F6_Bat_Over_V == 3)  //�ܵ�ѹ��ѹ3��
    ||(Error_Group4.Bit.F4_Bat_Under_V == 3)  //�ܵ�ѹǷѹ3��
    //||(Error_Group3.Bit.F6_DisChg_Over_I == 3) //�ŵ����3��
    //||(Error_Group5.Bit.F0_FeedB_Over_I == 3)   //��������3��
    ||(Error_Group1.Bit.F4_Ch_Over_I == 3)      //������3��
    ||(Error_Group2.Bit.F0_Cell_Over_V == 3)    //�����ѹ��ѹ3��
    ||(Error_Group2.Bit.F2_Cell_Under_V == 3)   //�����ѹǷѹ3��
    ||(Error_Group5.Bit.F6_Cell_Under_T == 3)  //�����¶ȹ���3��
    ||(Error_Group2.Bit.F4_Temp_Over == 3)     //�����¶ȹ���3��
    ||(Error_Group1.Bit.F6_Ins_Err == 3)       //��Ե��
    ||(Error_Group0.Bit.F0_Fire_Warning==3)       //����Ԥ��3��
    ||(Error_Group3.Bit.F1_V_CAN_Err)//����ͨѶ���ϣ���ֱ�����������ܵ繭��VCU)
    ||(Error_Group3.Bit.F0_Sub_Com_Err)//����ͨѶ����
    //||(Error_Group6.Bit.F0_Power_Vol)//����CAN�ϱ�
    ||(Error_Group0.Bit.F2_Ele_Relay_Con ==3)//����������
    ||(ACCha_Flag_BST) //�����������쳣
    ||(RelayErrorPowerOff)//�̵����µ����
    )
    {
        status_group3.Bit.Fault_Level = 3;//���ع��ϣ�������Ϊ0
        status_group4.Bit.St_BMS_System = 1;//BMSϵͳ״̬λ ���� ֻ�г���3�������ϱ�
    } 
    else if((Error_Group4.Bit.F6_Bat_Over_V == 2)//�ܵ�ѹ��ѹ2��
    ||(Error_Group4.Bit.F4_Bat_Under_V == 2)     //�ܵ�ѹǷѹ2��
    ||(Error_Group3.Bit.F6_DisChg_Over_I == 2)   //�ŵ����2��
    ||(Error_Group5.Bit.F0_FeedB_Over_I == 2)    //��������2��
    ||(Error_Group1.Bit.F4_Ch_Over_I == 2)       //������2��
    ||(Error_Group2.Bit.F0_Cell_Over_V == 2)     //�����ѹ��ѹ2��
    ||(Error_Group2.Bit.F2_Cell_Under_V == 2)    //�����ѹǷѹ2��
    ||(Error_Group5.Bit.F4_Cell_Dif_V == 2)      //�����ѹ������2��
    ||(Error_Group5.Bit.F2_Cell_Dif_T == 2)      //�����¶Ȳ�����2��
    ||(Error_Group5.Bit.F6_Cell_Under_T == 2)    //�����¶ȹ���2��
    ||(Error_Group2.Bit.F4_Temp_Over == 2)       //�����¶ȹ���2��
    ||(Error_Group4.Bit.F0_SOC_Low == 2)         //SOC��2��
    ||(Error_Group1.Bit.F6_Ins_Err == 2)        //��Ե��
    ||(Error_Group0.Bit.F0_Fire_Warning==2)       //����Ԥ��2��
    //||(Error_Group6.Bit.F0_Power_Vol)//����CAN�ϱ�
    ||(Error_Group0.Bit.F2_Ele_Relay_Con ==2)//����������
    )
    {
        status_group3.Bit.Fault_Level = 2;//��΢����������50%
    } 
    else if((Error_Group4.Bit.F6_Bat_Over_V) //�ܵ�ѹ��ѹ1��
    ||(Error_Group4.Bit.F4_Bat_Under_V)      //�ܵ�ѹǷѹ1��
    ||(Error_Group3.Bit.F6_DisChg_Over_I)    //�ŵ����1��
    ||(Error_Group5.Bit.F0_FeedB_Over_I)     //��������1��
    ||(Error_Group1.Bit.F4_Ch_Over_I)        //������1��
    ||(Error_Group2.Bit.F0_Cell_Over_V)      //�����ѹ��ѹ1��
    ||(Error_Group2.Bit.F2_Cell_Under_V)     //�����ѹǷѹ1��
    ||(Error_Group5.Bit.F2_Cell_Dif_T)     //�����¶Ȳ�����1��
    ||(Error_Group5.Bit.F6_Cell_Under_T)   //�����¶ȹ���1��
    ||(Error_Group2.Bit.F4_Temp_Over)      //�����¶ȹ���1��
    ||(Error_Group4.Bit.F0_SOC_Low)        //SOC��1��
    ||(Error_Group1.Bit.F6_Ins_Err)         //��Ե��
    ||((g_caution_Flag_3 &0x02)!=0) //to PC   //��������������
    ||(Error_Group0.Bit.F0_Fire_Warning)       //����Ԥ��2��
    ||(Error_Group6.Bit.F0_Power_Vol)//����CAN�ϱ�
    )
    {
        status_group3.Bit.Fault_Level = 1;//һ��
    } 
    else 
    {
        status_group3.Bit.Fault_Level = 0;//�޹���
    }
}
//******************************************************************************
//* Function name:    FaultProcess
//* Description:      �����ֹ���: ������������Ϊ�����ع���
//* EntryParameter : None
//* ReturnValue    : ���޹��ϣ�0-�ޣ�1-��  
//******************************************************************************
unsigned char TaskFaultProcess(void) 
{
    unsigned char lever = 0;
    unsigned char i,j,k=0;
    static unsigned char caution1=0;
    static unsigned char caution2=0;
    static unsigned char caution3=0;
    static unsigned char caution4=0;
   
    if((stateCode==30)||(stateCode==110)||(stateCode==170))//
    {
        errorSystemVoltageOV();//�ܵ�ѹ��ѹ
        errorChargeOC();   //������ 
        if(stateCode==30) 
        {
            errorSystemVoltageUV();//�ܵ�ѹǷѹ
            errorDischargeOC();//�ŵ����
        }
        if(stateCode==170)
        {
            errorLowIsolation();//��Ե����
            CHG_SocketOT(); //���������� 
        }
    }
    if((g_BmsModeFlag == FASTRECHARGING)||(g_BmsModeFlag == RECHARGING))
    {
        DCChangerComError();//ֱ�����ͨ�Ź���
        Charge_Check();//�������������
    }
    
    if(g_BmsModeFlag == RECHARGING) //����ģʽ
    {
        ACChangerComError();
    }
    else if(g_BmsModeFlag == DISCHARGING)//�г�ģʽ 
    {
        errorCellVoltageUV();//�����ѹ���� 
        errorSOCLow();//SOC��
        VCUComError();//��VCUͨѶ����
    }                        
    innerCommOT3();//�ڲ�ͨ�Ź��� 
    errorCellVoltageOV(); //�����ѹ���� 
    errorCurrSensorIniatial(); //��������������
    errorCellUnbalance();//�����ѹ������
    errorTemUnbalance();//�����¶Ȳ�����
    errorCellTemperatureOT();//�����¶ȹ���
    errorCellTemperatureUT();//�����¶ȹ���
    Fire_Warning();//����Ԥ��
    PowerSupplyError();//�����Դ�쳣
    
    
    FaultLevel(); //���ϵȼ��ж�

    BMSProtect();////////BMS���ϼ̵���

    //*****************************************************************************************
    //******************************************************************************************
    ////////////��������֮ǰ��ȣ�û�б仯ʱ�������д洢��ֻ��1���ӲŴ洢������и��£��Ž��д洢
    if((caution1^g_caution_Flag_1)||(caution2^g_caution_Flag_2)||(caution3^g_caution_Flag_3)||(caution4^g_caution_Flag_4))
        StoreSysVariable();//����SOCֵ�͹�����Ϣ
    caution1 =  g_caution_Flag_1;
    caution2 =  g_caution_Flag_2;
    caution3 =  g_caution_Flag_3;
    caution4 =  g_caution_Flag_4;
    //////////////////////////////////////////////////////////////////
    ////Caution_Flag_4��Bit 0����ѹĸ�����ӹ��ϣ�Bit 1�����������ϣ�    
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
//* Description:     BMS���ı���,����״̬�����ƣ�ǿ�ƶϸ�ѹ
//* EntryParameter : �Ͽ����м̵���
//* ReturnValue    : void
//******************************************************************************
int TurnOffAllRelay(void)//
{
    openNegRelay();
    delay(25000); //19ms
    openPosRelay();
    delay(25000); //19ms
    TurnOff_INA2K();//���̵���
    delay(25000); //19ms
    TurnOff_INA1K();//���̵���
    delay(25000); //19ms
    TurnOff_INBK();//���̵���
    delay(25000); //19ms
    TurnOff_INHK();//���̵���
    delay(25000); //19ms
    Error_Group6.Bit.F3_BMS_Protect = 1;//����CAN��ֵ BMS���޹������ұ���
}
//********************************************************************************************
//***********************************the end*************************************************
//********************************************************************************************
//********************************************************************************************