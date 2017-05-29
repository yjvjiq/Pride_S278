//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : S133
//* File Name          : SocAdjust.c
//* Author             : judy
//* Version            : V1.0.0
//* Start Date         : 2016.2.14
//* Description        : ���ļ����ڵ����ڲ�ͬ�����SOCֵ
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

#include "BMS20.h"

//******************************************************************************
//* Function name:   SocRechargeAdjust
//* Description:     ���ʱSOC����
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void SocRechargeAdjust(void)//���ĩ��SOC�����ӳ���
{
    if(g_highestCellVoltage<3.50)
        return;
    g_socValue=1;
}

//******************************************************************************
//* Function name:   SocEndDischargeAdjust
//* Description:     �ŵ�ĩ��SOC����
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void SocEndDischargeAdjust(void)//�ŵ�ĩ��SOC�����ӳ���
{
    float x1;//�洢��ǰ�¶�������SOC�ĵ����ѹ
    static unsigned int time10S=0;
    static unsigned int counter5S1=0;
    static unsigned int counter5S2=0;    
    
    if((g_lowestTemperature>=45) && (g_lowestCellVoltage<=2.9)) //�����С��5��
    {
        time10S++;
        if(time10S>=1400)   //
        {
            time10S=1400;
            g_socValue=0.1;
        }
    } 
    else
        time10S=0;
    
    if(g_lowestTemperature>=45) //�����С��5��
    {
        if((g_systemCurrent<=60)&&(g_lowestCellVoltage<=2.6)) 
        {
            counter5S1++;
            if(counter5S1>=700) 
            {
                g_socValue=0;
            }
        } 
        else
            counter5S1=0;
    } 
    else
    {
        if((g_systemCurrent<=60)&&(g_lowestCellVoltage<=2.5)) 
        {
            counter5S2++;
            if(counter5S2>=700) 
            {
                g_socValue=0;
            }
        } 
        else
            counter5S2=0;
    }
    
}
//******************************************************************************
//* Function name:   SocProDischargeAdjust
//* Description:     �ŵ������SOC����
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void SocProDischargeAdjust(void)//�ŵ������SOC�����ӳ���(ֻ�����г�ģʽ������)
{
    static unsigned int Time2min;
    float Cap1,Cap2;
   
    if(g_systemCurrent>3)
    {
      
        Time2min=0;
        return;
    } 
    else
    {
        Time2min++;
        if(Time2min>100)//2min
        {
            //Cap1=CapacityCalculate(g_lowestCellVoltage);
            //g_socValue=Cap1/g_realNominalCap;
            Time2min=0;
        }
    }
    
}
//******************************************************************************
//* Function name:   SocOCVAdjust
//* Description:     ���ݵ�ǰ�����ѹ״��������ǰ��SOC
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void SocOCVAdjust(void)
{
    float capMin=0;
    float capMax=0;
    float socOcv =0;
    unsigned int timenow,timeold;

    
    ////Read out the previous power-off time, compare it to the current time, 
    ////if the time is over 2 hours, then adjust the SOC 
	  
	  ReadOutErrorRecord(g_errorCounter);//Ϊ�˵õ��ϴ��µ�ʱ��ʱ��
	  timenow=CurrentTime[4]*1440+CurrentTime[2]*60+CurrentTime[1];
	  timeold=g_storageSysVariableOut[SYS_REALTIME_DAY]*1440+g_storageSysVariableOut[SYS_REALTIME_HOUR]*60+g_storageSysVariableOut[SYS_REALTIME_MINUTE];
	  //if((timenow-30)<timeold) //���ʱ���Ƿ񳬹�30minutes
	  //    return;
	  
	   if(g_lowestCellVoltage>=3.34)//
	   {
	       if(g_socValue<0.95)
	           g_socValue=0.95;
	   } 
	   else if(g_lowestCellVoltage<2.9)
	       g_socValue=0;
	   else if(g_lowestCellVoltage<3.1)
	       g_socValue=0.05;

}
//*******************************************************************************
//***********************the end**************************************************
//********************************************************************************
//********************************************************************************
//********************************************************************************




