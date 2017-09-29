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
    
    if((g_highestCellVoltage >=3.65)&&(g_highestCellVoltage<=4.0))
    {
        g_socValue=1;
		Can_g_socValue = 1;
        g_leftEnergy = g_socValue*(g_realNominalCap*3600);	  
        g_energyOfUsed = 0;
        AdjustQ1Value();
    }
}
//******************************************************************************
//* Function name:   SocEndDischargeAdjust
//* Description:     �ŵ�ĩ��SOC����
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void SocEndDischargeAdjust(void)//�ŵ�ĩ��SOC�����ӳ���
{
    //float x1;//�洢��ǰ�¶�������SOC�ĵ����ѹ
    static unsigned int time10S=0;
    static unsigned int counter5S1=0;
    static unsigned int counter5S2=0;
	
    if((g_lowestTemperature>=(40+5))&&(g_lowestCellVoltage!=0))//Tmin<5
    {
        if((g_lowestCellVoltage<=2.6)&&(g_systemCurrent<=0.5*SetCap))
        {
            counter5S1++;
            if(counter5S1>=700)   // 5S
            {
                counter5S1=707;
                Can_g_socValue_Start=0; //SOC����Ϊ0
	              CanSocIntegral();
            }
        }
        else if(g_lowestCellVoltage<=2.9)
        {
            time10S++;
            counter5S1=0;
            if((time10S>=1400)&&(Can_g_socValue>0.1))   // 10s
            {
                time10S=1400;
                Can_g_socValue_Start=0.1;//SOC����Ϊ10%
	              CanSocIntegral();
            }
        }
        else
        {
            time10S=0;
            counter5S1=0;
        }
    } 
    else if((g_lowestTemperature<(40+5))&&(g_lowestCellVoltage!=0)) //g_systemCurrent
    {
        if((g_lowestCellVoltage<=2.5)&&(g_systemCurrent<=0.5*SetCap))
        {
            counter5S2++;
            if(counter5S2>=700)   //5S
            {
				counter5S2=707;
				Can_g_socValue_Start=0;
				CanSocIntegral();
            }
        } 
        else
            counter5S2=0;     
    }
    /*
    if((g_lowestCellVoltage<=3.0)&&(Can_g_socValue>=0.02))
    {
        counter5S1++;
        if(counter5S1>=700)
        {   
            Can_g_socValue_Start =0.02;
            CanSocIntegral();
            counter5S1=0;
        }
    }
	*/
}
//******************************************************************************
//* Function name:   SocOCVAdjust
//* Description:     ���ݵ�ǰ�����ѹ״��������ǰ��SOC
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
//long timenow=0;
//long timeold=0;
void SocOCVAdjust(unsigned int delaytime)
{
    unsigned int timenow=0;
    unsigned int timeold=0;
	
	ReadOutErrorRecord(g_errorCounter);//Ϊ�˵õ��ϴ��µ�ʱ��ʱ��
	timenow=CurrentTime[4]*1440+CurrentTime[2]*60+CurrentTime[1];
	timeold=g_storageSysVariableOut[SYS_REALTIME_DAY]*1440+g_storageSysVariableOut[SYS_REALTIME_HOUR]*60+g_storageSysVariableOut[SYS_REALTIME_MINUTE];
	if(timenow < (timeold + delaytime)) //���ʱ��û�г������ʱ�� 30min
	{
		//OCVState = 0;
		return;
	}
	
	if((g_lowestCellVoltage==0)||(g_highestCellVoltage>=5.0))//��ֹ�����ѹΪ0������ߵ��������
		return;
	
	if(g_lowestCellVoltage>=3.34)
	{
		if(g_socValue<0.95)
		{	        
			g_socValue=0.95;
			g_leftEnergy = g_socValue*(g_realNominalCap*3600);
			g_energyOfUsed = 0; 
			//OCVState = 1;
		}
	}
	else if(g_lowestCellVoltage<2.9)
	{	    
		g_socValue=0;
		g_leftEnergy = g_socValue*(g_realNominalCap*3600);	  
		g_energyOfUsed = 0;
		//OCVState = 1;
	}
	else if(g_lowestCellVoltage<3.1)
	{	    
		g_socValue=0.05;
		g_leftEnergy = g_socValue*(g_realNominalCap*3600);	  
		g_energyOfUsed = 0;
		//OCVState = 1;
	}
}
//*******************************************************************************
//***********************the end**************************************************
//********************************************************************************
//********************************************************************************
//********************************************************************************




