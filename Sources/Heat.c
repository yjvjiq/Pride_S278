//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : �ƺ�10.5m
//* File Name          : Heat.c
//* Author             : ����ѩ
//* Version            : V1.0.0
//* Start Date         : 2016.9.6
//* Description        : ���ļ����ڼ��ȹ���
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include  "BMS20.h"
unsigned char st_heating;//������ؼ���״̬��0δ���ȣ�1Ԥ�����У�2�߳��߼�����
float HeatCurt;
unsigned char heatingStart=0;//Ԥ����ֹͣ��־λ
unsigned char BeforeTempFlag1=0;//�ϵ�ǰ�¶��ж�<0
unsigned char BeforeTempFlag2=0;//�ϵ�ǰ�¶��ж� <10
//unsigned char BeforeTempFlag3=0;//�ϵ�ǰ�¶��ж� >10
//***********************************************************************
//* Function name:   HeatManage
//* Description:     ״̬��Ϊ100��110��170��ʱ������ȹ���
//* EntryParameter : None
//* ReturnValue    : None
//************************************************************************
void HeatManage(void)
{
    static unsigned int tt=0;
    
    if(Tavg==0)
    {
        st_heating==0;  
        return;
    }
    if(g_highestCellVoltage>HIGHEST_ALLOWED_CHARGE_CV)
        return;
    //************δ���Ƚ׶�,�¶������Ǵ��ڵ���10��st_heating��Ϊ0
    if(st_heating==0)
    {
        //if(Tavg<40)//0+40,С��0�ȣ�����Ԥ���Ƚ׶�
        if(Tavg<=(START_ALLOWED_PreHEAT_CHARGE_TEM+40))//С��0�ȣ�����Ԥ���Ƚ׶� ֻ���Ȳ����
        {
            st_heating=1;
            HeatCurt = HIGHEST_ALLOWED_HEAT_CHARGE_C;
            BeforeTempFlag1=1;
            heatingStart=1;
   
        } 
        else if(Tavg<=(START_ALLOWED_HEAT_CHARGE_TEM+40))//10������߼��ȱ߳��׶�
        {
            st_heating=2;
            HeatCurt=HIGHEST_ALLOWED_HEAT_CHARGE_C;
            BeforeTempFlag2=1;
  
        }
    }
    //*************��Ԥ���Ƚ׶Σ�ֻ���ȣ��������ֻ������ȵ����������������
    if(st_heating==1)
    {
        if(Tavg>(STOP_ALLOWED_PreHEAT_CHARGE_TEM+40)) //���ڵ���5�ȡ㣬Ԥ���Ƚ���
        {
            st_heating=2;
            HeatCurt=HIGHEST_ALLOWED_HEAT_CHARGE_C;
            heatingStart=0;
        }
    }
    //*************�ڱ߼��ȱ߳��׶�   
    if(st_heating==2)
    {
        if(Tavg>(STOP_ALLOWED_HEAT_CHARGE_TEM+40)) //���ڵ���15�㣬���ȱ߳��׶ν���
        {
            st_heating=0;
            HeatCurt=0;
            BeforeTempFlag1=0;
            BeforeTempFlag2=0;
        }
        if(Tavg<(BACK_HEAT_TEM+40))  //С��-1�ȣ�����ֻ���Ƚ׶�
        {
            st_heating=1;
            HeatCurt = HIGHEST_ALLOWED_HEAT_CHARGE_C;
        }
    }
}
//******************************************************************************
//* Function name:   HeatAndChargeControl
//* Description:     �߳��߼��ȼ��ȼ̵�������
//* EntryParameter : void
//* ReturnValue    : void
//******************************************************************************
void HeatAndChargeControl(void)
{
   
    static unsigned int time;
    /*if(st_heating==0) //
    {
        TurnOff_INHK();//�Ͽ����ȼ̵���
    } 
    else if(st_heating==1)  //����Ԥ����
    {
        TurnOn_INHK(); //�պϼ��ȼ̵��� 
    }*/
    if(HeatFlag==0) //
    {
        TurnOff_INHK();//�Ͽ����ȼ̵���
    } 
    else if(HeatFlag==1)  //����Ԥ����
    {
        TurnOn_INHK(); //�պϼ��ȼ̵��� 
    }
    else if(HeatFlag==3)  //�Ͽ�����
    {
        TurnOff_INHK(); //�Ͽ�����
    }   
    else
    {
        if(Tavg>(STOP_ALLOWED_HEAT_CHARGE_TEM+40)) //����¶ȴ���40+15�ȣ���Ͽ����ȼ̵���
        {
            TurnOff_INHK();  
        }
        else if(Tavg<=(START_ALLOWED_HEAT_CHARGE_TEM+40))//����¶�С��40+10�ȣ���պϼ��ȼ̵���
        { 
            TurnOn_INHK();
        }
        if((g_systemCurrent>=2)&&(g_lowestCellVoltage<2.8))//��ֹ�õ�ؽ��м���
        {      
            time++;
            if(time>100)//250ms*100=25S
                TurnOff_INHK();        
        } 
        else
            time=0;
    }
}