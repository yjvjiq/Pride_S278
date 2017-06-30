//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : S133
//* File Name          : Connect.c
//* Author             : judy
//* Version            : V1.0.0
//* Start Date         : 2016.2.14
//* Description        : 该文件用于判断系统当前连接的是放电，慢充还是快充状态
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h"
//******************************************************************************
//* Function name:   bmsModeAwake
//* Description:     上电唤醒信号判断，得出系统工作模式
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void bmsModeAwake(void) 
{
    //long timedelay = 6500000;
    if(input5_state()==0)//CC2状态检测,IN5==0,快充枪已插;IN5==1,快充枪没有插
    {
        while(input6_state())
            _FEED_COP(); //无charge_in信号，则等            
        if((CC2VOL <= CC2VOLHIGH) && (CC2VOL >= CC2VOLLOW))
        {
            g_BmsModeFlag = FASTRECHARGING; //快充模式   
            plug_DC_Connect=1;
            status_group4.Bit.Mode_BMS_Work = 2;//充电状态 
            status_group3.Bit.St_CHG_Mode = 1;//充电模式等于DC充电
            status_group3.Bit.St_Charge = 1;//正在充电 
            InsRelayControl=1;//快充时采绝缘控制给SBMS
            stateCode=141;      
        }
        else
        {
            stateCode=187;
            plug_DC_Connect=0;//充电枪连接
        }
    } 
    else //如果IN5！=0，说明快充枪没插
    {
        //收到车载WiFi发送降弓开关，降弓到位信号
        //if((VCU_ChgControl.Bit.downC_Switch)&&(VCU_ChgControl.Bit.downC_OK))  //////////需要确认||(VCU_ParkBrake.Bit.Parking_Brake)
        if((VCU_ChgControl.Bit.downC_Switch)||(VCU_ChgControl.Bit.downC_OK))
        {
            g_BmsModeFlag = RECHARGING;//进入受电弓充电模式
            status_group4.Bit.Mode_BMS_Work = 2;//BMS当前工作状态=充电
            status_group3.Bit.St_CHG_Mode=2;//充电模式等于AC充电
            status_group3.Bit.St_Charge = 1;//正在充电 
            stateCode = 81;
            //InsRelayControl = 0;//受电弓时不采绝缘，控制指令发给SBMS
        } 
        else if(input4_state()==0)
        {
            g_BmsModeFlag = DISCHARGING;//放电模式
            acc_Connect=1;   //ON信号
            status_group4.Bit.Mode_BMS_Work = 1;//BMS当前工作状态=放电状态 
            stateCode=11;
            //InsRelayControl = 0;//放电时不采绝缘，控制指令发给SBMS 
        }
    }
}
//******************************************************************************
//* Function name:   SignalOnJudge
//* Description:     过程中,判断是否有插枪信号
//* EntryParameter : None
//* ReturnValue    : None
//* 注意           : 交流与直流时无法以ChangerIN判断信号,直流已CC2为信号,交流已报文CC的状态为信号;
//                 : 当检测到插枪信号后(直流或交流),需要等待ChangerIN信号后,才能进行充电上电                      
//******************************************************************************
void SignalOnOffJudge(void) 
{
    static unsigned char KEY_Connect=0;    //钥匙信号有
    static unsigned char AC_ConnectIN=0;   //交流ChangerIN连接信号有
    static unsigned char AC_Connect=0;     //交流充电枪连接信号有
    static unsigned char DC_Connect=0;     //直流充电枪连接信号有
    static unsigned char KEY_DisConnect=0; //钥匙信号无
    static unsigned char AC_DisConnectIN=0;//交流ChangerIN连接信号无
    static unsigned char AC_DisConnect=0;  //交流充电枪连接信号无
    static unsigned char DC_DisConnect=0;  //直流充电枪连接信号无
    long timedelay = 6500000;
    if( BootState == 1) //Boot
    {
        turnOffSW_Power();//close总电源开关
        //state_group4.Bit.Mode_BMS_Run = 3;//Boot To VCU
    } 
    else //非boot
    {
      ////////////////////////////直流充电枪检测////////////////////////
        if(input5_state()==0)//快充枪插上
        {
          ////////////////////直流充电枪CC2插枪检测/////////////////////
            DC_Connect++; //直流插枪延时检测,必须连续检测到200*5ms时间才可以
            DC_DisConnect = 0;
            if(DC_Connect>=100)
            {               
                /*while((DC_CC2Count==0)&&(timedelay>0))
                {
                    timedelay--;   
                    _FEED_COP(); //无charge_in信号，则等;
                }*/   
                
                plug_DC_Connect = 1;
                DC_Connect = 0;
                 
            }
        }
        else //CC2无信号
        {
         ////////////////////直流充电枪CC2拔枪检测/////////////////////   
            DC_Connect=0;
            DC_DisConnect++;//直流拔枪延时检测,必须连续检测到200*5ms时间才可以
            if(DC_DisConnect>=100)
            {
                InsRelayControl=0;//快充时采绝缘控制给SBMS 
                plug_DC_Connect=0;
                if(g_BmsModeFlag == FASTRECHARGING)
                {                  
                    fastendflag=1;
                    fastend2|=0x40;//充电连接器故障
                }
                DC_DisConnect=0;
            }
            
            //////////////////受电弓充电检测/////////////////////////////
            if((VCU_ChgControl.Bit.downC_Switch)&&(VCU_ChgControl.Bit.downC_OK))
            {
                AC_Connect++; //交流插枪延时检测,必须连续检测到200*5ms时间才可以
                AC_DisConnect = 0;
                if(AC_Connect>=100)
                {              
                    plug_AC_CP_Connect = 1;
                    status_group4.Bit.Mode_BMS_Work = 2;//BMS当前工作状态=充电
                    status_group3.Bit.St_CHG_Mode=2;//充电模式等于AC充电
                    AC_Connect = 0;
                }
            } 
            else
            {              
                AC_Connect = 0;
                AC_DisConnect++;//交流插枪延时检测,必须连续检测到200*5ms时间才可以
                if(AC_DisConnect>=100)
                {
                    plug_AC_CP_Connect = 0;
                    AC_DisConnect = 0;
                } 
                ////////////////////ACC钥匙开关ON检测/////////////////////  
                if(input4_state()==0)//如果ACC有信号,则On信号有效
                {
                    KEY_Connect++;
                    KEY_DisConnect = 0;
                    if(KEY_Connect>=100)
                    {                  
                        acc_Connect = 1;    //ON信
                        status_group4.Bit.Mode_BMS_Work = 1;//BMS当前工作状态=放电状态 
                        KEY_Connect = 0;
                    } 
                }
                else
                {
                ////////////////////ACC钥匙开关OFF检测/////////////////////
                    KEY_Connect = 0;
                    KEY_DisConnect++;
                    if(KEY_DisConnect>=100)
                    {
                        acc_Connect = 0;    //OFF信
                        KEY_DisConnect = 0;                
                    }
                }
            } 
      
        }    
        ////////////////////ChangerIN检测/////////////////////     
        if(input6_state()==1)
        {
            AC_ConnectIN = 0;
            AC_DisConnectIN++;//交流插枪延时检测,必须连续检测到200*5ms时间才可以
            if(AC_DisConnectIN>=100)
            {
                ChangerINError = 1;
                AC_DisConnectIN = 0;
            }
        } 
        else 
        {
        
            AC_DisConnectIN = 0;
            AC_ConnectIN++;//交流插枪延时检测,必须连续检测到200*5ms时间才可以
            if(AC_ConnectIN>=100)
            {
                ChangerINError = 0;
                AC_ConnectIN = 0;
            }        
        }
        
  
    }
}
//*************************************************************************
//***************************************************************************
//*******************************************************************************
//**********************************************************************************
//************************************************************************************