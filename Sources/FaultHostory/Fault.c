//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : Fault.c
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2014.5.7
//* Description        : ���ļ� ��ϵͳ�ĸ��������ж�
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h" 
///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 

//***********************************************************************************
//***********************************************************************************
unsigned char  g_caution_Flag_1 =0;//BMS����״̬ FOR ��λ��
unsigned char  g_caution_Flag_2 =0;
unsigned char  g_caution_Flag_3 =0;
unsigned char  g_caution_Flag_4 =0;
unsigned int g_errorCounter;

//**********************************************************************
//* Function name:   CarFaultDone
//* Description:     �г������й��ϵĴ���
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void CarFaultDone()
{
    
    //////////////////////////////*�ϸ�ѹ*/////////////////////////////
    //////////////////////3�����ϵ�ذ���ѹ,�����ѹ//////////////////////////////
    if(((CutChaCurt0.word & 0xdd0f)!=0)||(CutDisCurt0.word!=0))  
        OffState=1;//�����µ�//ȥ����罵��Ϊ0�ĵ����ѹ2�����ܵ�ѹ��ѹ2��
    //////////////////////////////*�ϸ�ѹ*/////////////////////////////
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
    if(g_BmsModeFlag != DISCHARGING)
    {      
        if((g_lowestCellVoltage<=2.45)&&(g_lowestCellVoltage!=0))
        {
            LCDelaytime++;
            if(LCDelaytime>20)
            {
                LCDelaytime=27;
                openNegRelay();
                delay(25000); //19ms
                TurnOff_INA2K();//���̵���
                
            }
        } 
        else
        {
            LCDelaytime=0;
        }
    }
    if((g_BmsModeFlag == RECHARGING)||(g_BmsModeFlag == FASTRECHARGING))
    {      
        if(g_highestCellVoltage>=3.7)//10s
        {
            HCDelaytime++;
            if(HCDelaytime>30)
            {
                HCDelaytime=37;
                openNegRelay();
                delay(25000); //19ms
                TurnOff_INA2K();//���̵���
            }
        } 
        else
        {
            HCDelaytime=0;
        }
        if(g_highVoltageV1>=666)//3.7*114=421.8
        {
            HBaDelaytime++;
            if(HBaDelaytime>30)
            {
                HBaDelaytime=37;
                openNegRelay();
                delay(25000); //19ms
                TurnOff_INA2K();//���̵���
            }
        } 
        else
        {
            HBaDelaytime=0;
        }
        
    }
    if(g_highestTemperature>95)//10s
    {
        HTDelaytime++;
        if(HTDelaytime>30)
        {
            HTDelaytime=37;
            openNegRelay();
            delay(25000); //19ms
            TurnOff_INA2K();//���̵���
        }
    } 
    else
    {
        HTDelaytime=0;
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
   
    //BMSProtect();////////BMS���ϼ̵���
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
    ////Caution_Flag_4��Bit 0����ѹĸ�����ӹ��ϣ�Bit 1����������ϣ�    
    ///////////////////  
    g_storageSysVariable[PARA_ERROR_LEVER] = lever;
    g_storageSysVariable[CAUTION_FLAG_1] = g_caution_Flag_1;	
    g_storageSysVariable[CAUTION_FLAG_2] = g_caution_Flag_2;	
    g_storageSysVariable[CAUTION_FLAG_3] = g_caution_Flag_3;	
    g_storageSysVariable[CAUTION_FLAG_4] = g_caution_Flag_4;	
  
	  return g_storageSysVariable[PARA_ERROR_LEVER];
}
//********************************************************************************************
//***********************************the end*************************************************
//********************************************************************************************
//********************************************************************************************