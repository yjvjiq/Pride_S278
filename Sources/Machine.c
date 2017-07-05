//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : PHEV.DJ
//* File Name          : Machine.c
//* Author             : judy
//* Version            : V1.0.0
//* Start Date         : 2011.6.7
//* Description        : 该文件用于项目的状态机策略处理
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h" 

//******************************************************************************
//******************************************************************************
//******************************************************************************

unsigned int tmr_p1 =0;
unsigned int tmr_p2 =0;
unsigned int tmr_p3 =0;
unsigned int tmr_p4 =0;
unsigned int tmr_p5 =0;
unsigned int tmr_p6 =0;

unsigned char prechargeFinished =0;      //预充电完成标志位
unsigned char preChargeON =0;
unsigned char bmsSelfcheckCounter=0;
unsigned char acc_Connect=0;
unsigned char MSDError=0;                 //MSD故障
unsigned char N_RelayConnetError=0;       //负极继电器粘连
unsigned char N_RelayDisConError=0;       //负极继电器断路
unsigned char PreCha_RelayConError=0;     //预充继电器粘连
unsigned char P_RelayConError=0;          //正极继电器粘连
unsigned char PreCha_RelayDisConError=0;  //预充继电器断路
unsigned char P_RelayDisConError=0;  //正极继电器断路
unsigned char CCHG_RelayDisConError=0;  //受电弓充电继电器断路
unsigned char CCHG_RelayConError=0;  //受电弓充电继电器粘连 OK

unsigned char CHG_N_RelayConError=0;  //充电负继电器粘连  OK
unsigned char CHG_N_RelayDisConError=0;  //充电负继电器断路  OK

unsigned char DCCHG_RelayConError=0;  //快充继电器粘连
unsigned char DCCHG_RelayDisConError=0;  //快充继电器断路
unsigned char Cha_ResDisConError=0;       //预充电阻断路
unsigned char PreCha_Error=0;             //预充电失败
unsigned char SelfCheck = 0;              //BMS自检结果: 2,未通过;1通过(预充继电器是否成功断开)
unsigned char RelayErrorPowerOff = 0;     //继电器下电故障


//******************************************************************************
//* Function name:   VehiclePowerOffLogic
//* Description:     BMS发生故障，下电
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void VehiclePowerOffLogic(void)
{
    static unsigned char OffFlag=0;
    static unsigned char count=0;
    
    TurnOff_INZK();//断开正极接触器 
    turnOffSW_Power();//关闭电源开关彻底断电
}
//******************************************************************************
//* Function name:   closeNegRelay
//* Description:     闭合主负继电器
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void closeNegRelay(void) 
{
    TurnOn_INFK();                 //to pc
    delay(25000);                  //19ms
    delay(25000);                  //19ms
}
//******************************************************************************
//* Function name:   openNegRelay
//* Description:     断开主负继电器
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void openNegRelay(void) 
{
    TurnOff_INFK();//断开主负继电器
    //state_group1.Bit.St_N_Relay=0;//to vcu
    delay(25000); //19ms
    delay(25000); //19ms
    status_group3.Bit.St_N_Relay=0;  

}
//******************************************************************************
//* Function name:   closePosRelay
//* Description:     闭合主正继电器
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void closePosRelay(void) 
{
    TurnOn_INZK();//闭合主正继电器
    delay(25000); //20ms
    delay(25000); //20ms
    status_group3.Bit.St_P_Relay=1;//to vcu
}
//******************************************************************************
//* Function name:   openPosRelay
//* Description:     断开主正继电器
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void openPosRelay(void) 
{
    TurnOff_INZK();//断开主正继电器
    delay(25000); //20ms
    //TurnOff_INBK();//断开预充电继电器
    //delay(25000); //20ms
    status_group3.Bit.St_P_Relay=0;//to vcu
}

//******************************************************************************
//* Function name:   GetTavg
//* Description:     充放电功率参考的温度取值
//* EntryParameter : 最高温度，最低温度
//* ReturnValue    : Tavg
//******************************************************************************
/*unsigned char GetTavg(unsigned char LowTem,unsigned char HighTem)  
{
    unsigned char Tavg1;

    if((LowTem>10+40)&&(HighTem>=55+40))
        Tavg1=HighTem;
    
    else if((LowTem>10+40)&&(HighTem<55+40)) //测试完需要放开
        Tavg1=g_averageTemperature;
    
    else if(LowTem<=10+40) 
        Tavg1=LowTem;
    
    return Tavg1;
} */
//******************************************************************************
//* Function name:   GetTavgProcess
//* Description:     充放电功率参考的温度取值
//* EntryParameter : 最高温度，最低温度
//* ReturnValue    : Tavg
//******************************************************************************
void GetTavgProcess(void)  
{
    static unsigned char timer1,timer2; 
    if(g_highestTemperature>45+40)//若是最高温度大于45度持续2S，那么平均温度为最高温度
    {
        timer2=0;
        timer1++;
        if(timer1>7)//300ms*7=2100ms
        {
            Tavg=g_highestTemperature;
            timer1=0;
        }
    } 
    else// if(g_highestTemperature<=45+40)//若是最高温度小于45度持续2S，那么平均温度为最低温度
    {     
        timer1=0;
        timer2++;
        if(timer2>7) 
        {
            Tavg=g_lowestTemperature;
            timer2=0;
        } 
    }
}
//******************************************************************************
//* Function name:   HighVoltDetectPart1
//* Description:     整车state机制上电步骤1：状态机编码12/17or46/126
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void HighVoltDetectPart1(void) 
{
    static unsigned char tt=0;
    static unsigned char pp=0;
    static unsigned char pc=0;
	static U16 s_detect_part1_cnt_t = 0;

	s_detect_part1_cnt_t++; // record how many times run HighVoltDetectPart1 to cover the fault detect.
	
    //**************检测MSD******************************//////////////////////// 
    if(g_highVoltageV1 < 100) 
    {                       
        tt++;   
        if(tt>=12)//60ms才能判断出来
        {
            //to vcu
            RelayErrorPowerOff = 1;//继电器下电故障
            MSDError = 1;
            g_caution_Flag_4 |=0x01; //to PC
            tt=13;
        }
    } 
    else   
    {
        tt=0;
    }
    
    //********检测主负继电器粘连 *****************//////////////////////////////////
    if(g_BmsModeFlag == DISCHARGING)
    {
        if(g_highVoltageV2 >= 200)
        {
            pp++;
            if(pp>=12)//60ms
            {
                /////////////////负极粘连///////////////////
                RelayErrorPowerOff = 1;//继电器下电故障
                BmsCtlStat0 |= 0x01;//负极继电器状态闭合
                g_caution_Flag_4 |= 0x02; //to PC
                Error_Group3.Bit.F4_N_Con_Err = 1;//error to VCU
                N_RelayConnetError = 1;
                pp = 13;
                /////////////////负极粘连///////////////////
            }
        }
        else
        {
             pp=0;
        }
    }
	
    //********检测充电负继电器粘连 *****************//////////////////////////////////
    if((g_BmsModeFlag == RECHARGING)||(g_BmsModeFlag == FASTRECHARGING))
    {
        if(g_highVoltageV4>=200)
        {
            pc++;
            if(pc>=20)//100ms
            {
                /////////////////充电负极粘连///////////////////
                RelayErrorPowerOff = 1;//继电器下电故障
                Error_Group1.Bit.F2_DCChg_Neg_Relay_Con = 1;//直流充电负粘连报警位也作为受电弓负
                CHG_N_RelayConError = 1;
                pc=21;
                /////////////////充电负极粘连///////////////////
            }
        } 
        else
        {
             pc=0;
        }
    }

    if(MSDError==0 && s_detect_part1_cnt_t >= 25)//all BMS mode need detect.
    {
        if(((N_RelayConnetError==0)&&(stateCode == 12))//行车
          ||((CHG_N_RelayConError==0)&&(stateCode == 82))//受电弓
          ||((CHG_N_RelayConError==0)&&(stateCode == 142)))//快充
        {
            bmsSelfcheckCounter=1; //正常状态 
        } 
    }
}
//******************************************************************************
//* Function name:   HighVoltDetectPart2
//* Description:     高压检测：状态机编码17/87负极继电器断路 或预充电阻断路故障
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void HighVoltDetectPart2(void)//预充继电器已经闭合 
{    
    static unsigned char NDisConnect_tt=0;
    static unsigned char PConnect_tt=0;
    static unsigned char CCHGConnect_tt=0;
    static unsigned char DCCHGConnect_tt=0;
    static unsigned char CHGDisConnect_tt=0;
	static U16 s_detect_part2_cnt_t = 0;

	s_detect_part2_cnt_t++;
	
    if((tmr_p2 <= 60)) //延时20ms或者大于500ms,不在运行 	//||(tmr_p2 >= 600)
        return;

    if(g_BmsModeFlag == DISCHARGING)//行车
    {
		/////负极断路/////// 
        if(g_highVoltageV2<200) 
        {
            NDisConnect_tt++;
            if (NDisConnect_tt>=12)//滤波延时60ms，电压是否能及时变化？
            {
                RelayErrorPowerOff = 1;//继电器下电故障
                g_caution_Flag_4 |=0x04; //to PC
                BmsCtlStat0 &=0xfe;//负极接触器状态断开
                N_RelayDisConError = 1;  //负极继电器断路
                NDisConnect_tt = 13;
            }
        }
        else   
        {
            NDisConnect_tt=0;  
        }
        
        /////正极粘连//////
        if(g_highVoltageV3>200) 
        {
            PConnect_tt++;
            if (PConnect_tt>=12)//滤波延时60ms，电压是否能及时变化？
            {
                RelayErrorPowerOff = 1;//继电器下电故障
                g_caution_Flag_4 |= 0x10; //to PC
                BmsCtlStat0 |= 0x02;//正极继电器状态闭合
                Error_Group3.Bit.F2_P_Con_Err = 1;//error to VCU
                P_RelayConError = 1;//正极粘连故障
                PConnect_tt = 13;
            }
        }
        else   
        {
            PConnect_tt=0;  
        }
    }  
    else if((g_BmsModeFlag == RECHARGING)||(g_BmsModeFlag == FASTRECHARGING))//快慢充 
    {
		/////充电负继电器断路///////充电都检
		if(g_highVoltageV4<200) 
		{
			CHGDisConnect_tt++;
			if (CHGDisConnect_tt>=20)//滤波延时60ms，电压是否能及时变化？
			{
				RelayErrorPowerOff = 1;//继电器下电故障
				CHG_N_RelayDisConError = 1;//充电负断路故障
				CHGDisConnect_tt = 23;
            }
        }
        else   
        {
            CHGDisConnect_tt=0;  
        }
        
        if(g_BmsModeFlag == RECHARGING)
        {
            /////受电带充电继电器粘连///////
            if(g_highVoltageV6>200)  
            {
                CCHGConnect_tt++;
                if (CCHGConnect_tt>=20)//滤波延时60ms，电压是否能及时变化？
                {
                    RelayErrorPowerOff = 1;//继电器下电故障
                    g_caution_Flag_2 |=0x20; //to PC
                    Error_Group1.Bit.F3_Ele_Relay_Con = 1;//error to VCU
                    status_group2.Bit.St_Ele_Relay = 2;//受电弓继电器连接
                    CCHG_RelayConError = 1;//受电弓充电继电器粘连
                    CCHGConnect_tt = 23;
                }
            }
            else   
            {
                CCHGConnect_tt=0;  
            }
        }
        else if(g_BmsModeFlag == FASTRECHARGING)
        {
			/////充电继电器粘连///////
			if(g_highVoltageV5>200)  
			{
				DCCHGConnect_tt++;
				if (DCCHGConnect_tt>=20)//滤波延时60ms，电压是否能及时变化？
				{
					RelayErrorPowerOff = 1;//继电器下电故障
					g_caution_Flag_3 |=0x80; //to PC
					Error_Group3.Bit.F5_DC_Con_Err = 1;//error to VCU
					DCCHG_RelayConError = 1;//快充继电器粘连
					DCCHGConnect_tt = 23;
                }
            }
            else   
            {
                DCCHGConnect_tt=0;  
            }
        }
    }

	if(s_detect_part2_cnt_t >= 25)
    {
        if(((N_RelayDisConError==0)&&(P_RelayConError==0)&&(stateCode == 17))//负极断路&&正极粘连 
        ||((CHG_N_RelayDisConError==0)&&(CCHG_RelayConError==0)&&(stateCode == 87))//充电负断路&& 受电弓充电粘连
        ||((CHG_N_RelayDisConError==0)&&(DCCHG_RelayConError==0)&&(stateCode == 147)))//充电负断路&& 快充粘连
        {
            bmsSelfcheckCounter=2; //正常状态 
        }
    }
}
//******************************************************************************
//* Function name:   HighVoltDetectPart3
//* Description:     整车state机制上电步骤3：状态机编码20/90
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void HighVoltDetectPart3(void) 
{      
    static unsigned char PDisConnect_tt=0;
    static unsigned char CCHGDisConnect_tt=0;
    static unsigned char DCHGDisConnect_tt=0;
    static U16 s_detect_part3_cnt_t = 0;

	s_detect_part3_cnt_t++;
	
    if(tmr_p3 <= 60)  //延时60ms
       return;
    
//    if(tmr_p3 <= 500)
//    {
        ///////////////正极继电器断路///////
  
        if((g_highVoltageV3 < 200)&&(stateCode == 20))
        {
            PDisConnect_tt ++;
            if (PDisConnect_tt>=12)//滤波延时60ms，电压是否能及时变化？
            {
                RelayErrorPowerOff = 1;//继电器下电故障
                g_caution_Flag_4 |=0x40; //to PC
                BmsCtlStat0 &=0xfd;//正极继电器状态断开
                P_RelayDisConError = 1;//正极断路故障
                PDisConnect_tt = 13;
            }
        }
        else   
        {
            PDisConnect_tt=0;  
        }
        /////////////受电带充电继电器断路///////
        if((g_highVoltageV6<200)&&(stateCode == 90)) 
        {
            CCHGDisConnect_tt ++;
            if (CCHGDisConnect_tt>=20)//滤波延时60ms，电压是否能及时变化？
            {
                RelayErrorPowerOff = 1;//继电器下电故障
                g_caution_Flag_2 |=0x40; //to PC
                CCHG_RelayDisConError = 1;//受电弓继电器断路故障
                status_group2.Bit.St_Ele_Relay = 1;//受电弓继电器未连接
                CCHGDisConnect_tt = 23;
            }
        }
        else   
        {
            CCHGDisConnect_tt=0;  
        }
         /////////////快充继电器断路///////
        if((g_highVoltageV5<200)&&(stateCode == 150)) 
        {
            DCHGDisConnect_tt ++;
            if (DCHGDisConnect_tt>=20)//滤波延时60ms，电压是否能及时变化？
            {
                RelayErrorPowerOff = 1;//继电器下电故障
                g_caution_Flag_3 |=0x40; //to PC
                DCCHG_RelayDisConError = 1;//充电继电器断路故障
                DCHGDisConnect_tt = 23;
            }
        }
        else   
        {
            DCHGDisConnect_tt=0;  
        }
//    } 
//    else 
    if(s_detect_part3_cnt_t >= 25) {
        if(((P_RelayDisConError==0)&&(stateCode == 20))
        ||((CCHG_RelayDisConError==0)&&(stateCode == 90))
        ||((DCCHG_RelayDisConError==0)&&(stateCode == 150)))
        {
            bmsSelfcheckCounter = 3; //正常状态
            status_group1.Bit.St_BMS = 0;//高压工作 
        }  
    } 
}  
//***********************************************************************
//************************************************************************
//*************************the end*************************************
//************************************************************************
