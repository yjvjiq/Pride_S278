//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : fotun_Logistics
//* File Name          : VehicleCAN.c
//* Author             : WsM
//* Version            : V1.0.0
//* Start Date         : 2016.7.8
//* Description        : 该文件根据项目通信协议发送相关的报文到整车或交流充电机
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h"


ERROR_GROUP0 Error_Group0;
ERROR_GROUP1 Error_Group1;
ERROR_GROUP2 Error_Group2;
ERROR_GROUP3 Error_Group3;
ERROR_GROUP4 Error_Group4;
ERROR_GROUP5 Error_Group5;
ERROR_GROUP6 Error_Group6;
ERROR_GROUP7 Error_Group7;

unsigned int Charge_Times;     //充电次数
unsigned long int chargeAH ;   //实际充电总安时
unsigned long int dischargeAH ;//实际放电总安时
float DC_ChargePower=0;//充电电量（输出电量）


/*futon Logistics use*/

float g_SOH=1;                                //SOH
                                             
float BiggestDischargeCurt = 0;              //最大放电电流
float BiggestFeedbackCurt  = 0;              //最大回馈电流
float BiggestDisCurtContinuous = 0;          //最大持续放电电流
float BiggestFeedbackCurtContinuous = 0;     //最大持续回馈电流

float BiggestDischargePower = 0;              //最大放电功率
float BiggestFeedbackPower  = 0;              //最大回馈功率
float BiggestDisPowerContinuous = 0;          //最大持续放电功率
float BiggestFeedbackContinuous = 0;          //最大持续回馈功率

unsigned int chargeRemainderTime = 0;          //剩余充电时间


float VehicleSpeed = 0;                         //车速信号快充                           
unsigned char WifeLife=0;  //wife信号生命信号 如果受电弓充电在快充CAN上，需要转发到整车CAN
                                                
VCU_REQUEST VCU_Request;
VCU_CELLREQUEST VCU_CellRequest;
VCU_CONTROL VCU_Control;
VCU_CHGCONTROL VCU_ChgControl;
VCU_PARKINGBRAKE VCU_ParkBrake;



STATUS_GROUP1 status_group1;
STATUS_GROUP2 status_group2;
STATUS_GROUP3 status_group3;
STATUS_GROUP4 status_group4;

/*futon Logistics use end*/

//*************************************************************************************
//*************************************************************************************
//*****************************以下是BMS给VCU发消息子程序******************************
//*************************************************************************************
//*************************************************************************************
//*************************************************************************************
//* Function name  : BMS_To_VCU_BasicMsg0
//* period         : 1000ms
//* Description    : 电池组基本信息3
//* EntryParameter : None
//* ReturnValue    : None
//*************************************************************************************
void BMS_To_VCU_BasicMsg0(void)
{
    
    struct can_msg mg;
    unsigned char tt=100;
    unsigned int buff;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
	  
	  buff = (unsigned int)(SYS_KWH*10); //电池组系统总能量138.2kwh=240*3.2*180
	  mg.data[0] = buff;   //低字节 
	  mg.data[1] = buff>>8;//高字节    
	  
	  buff = CELL_NUM;	//电池节数
	  mg.data[2] = buff;   //低字节 
	  mg.data[3] = buff>>8;//高字节 
	  	  
	  mg.data[4] = 0x00;  //    
	  mg.data[5] = 0x00;  //    
	 
    mg.data[6] = 0x00;  //
    mg.data[7] = 0x00;  //
     
    mg.id= 0x1825D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//*************************************************************************************
//* Function name  : BMS_To_VCU_BasicMsg1
//* period         : 1000ms
//* Description    : BMS_B_1
//* EntryParameter : None
//* ReturnValue    : None
//*************************************************************************************
void BMS_To_VCU_BasicMsg1(void)
{
    
    struct can_msg mg;
    unsigned char tt=100;
    unsigned int buff;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
	  
	  buff = (unsigned int)(HIGHEST_ALLOWED_CHARGE_V*10); //最高允许充电总电压
	  mg.data[0] = buff;   //低字节 
	  mg.data[1] = buff>>8;//高字节    
	  
	  buff = (unsigned int)(SYS_NOMINAL_V*10);	//电池组标称电压平台
	  mg.data[2] = buff;   //低字节 
	  mg.data[3] = buff>>8;//高字节 
	  	  
	  buff = (unsigned int)(HIGHEST_ALLOWED_CHARGE_T+40); //最高允许工作温度  
	  mg.data[4] = buff;   //低字节    
	  
	  buff = (unsigned int)(LOWEST_ALLOWED_WORK_T+40); //最低允许工作温度
	  mg.data[5] = buff;   //低字节    
	 
    mg.data[6] = 0x00;                  //预留
    mg.data[7] = BMU_NUMBER;                  //LECU个数 位BMU个数
     
    mg.id= 0x1826D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//*************************************************************************************
//* Function name  : BMS_To_VCU_BasicMsg2
//* period         : 1000ms
//* Description    : BMS_B_2
//* EntryParameter : None
//* ReturnValue    : None
//*************************************************************************************
void BMS_To_VCU_BasicMsg2(void)
{
    
    struct can_msg mg;
    unsigned char tt=100;
    unsigned int buff;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
	  
	  buff = (unsigned int)(HIGHEST_CELL_VOL*1000); // 单体电池最高保护电压
	  mg.data[0] = buff;   //低字节 
	  mg.data[1] = buff>>8;//高字节    
	  
	  buff = (unsigned int)(LOWEST_CELL_VOL*1000);	//单体电池最低保护电压
	  mg.data[2] = buff;   //低字节 
	  mg.data[3] = buff>>8;//高字节 
	  	  
	  buff = (unsigned int)(SYS_NOMINAL_AH*10); //电池组标称安时数
	  mg.data[4] = buff;   //低字节 
	  mg.data[5] = buff>>8;   //低字节     
	  
    mg.data[6] = g_SOH*100; //SOH值   
    mg.data[7] = CELL_TYPE|(COOL_TYPE<<4);                  //电池类型磷酸铁锂 3 自然冷却
     
    mg.id= 0x1827D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//*************************************************************************************
//* Function name  : BMS_To_VCU_CHGMsg1
//* period         : 500ms
//* Description    : BMS_C_1
//* EntryParameter : None
//* ReturnValue    : None
//*************************************************************************************
void BMS_To_VCU_CHGMsg1(void)
{
    
    struct can_msg mg;
    unsigned char tt=100;
    unsigned int buff;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
	  
	  buff = (unsigned int)(g_systemVoltage*10);
	  mg.data[0]= (unsigned char)buff;         // 充电电压低字节
    mg.data[1]= buff>>8;                     // 充电电压高字节
    
	  buff = (unsigned int)((g_systemCurrent+400)*10);
  	mg.data[2]= (unsigned char)buff;//充电电流低字节 
    mg.data[3]= buff>>8;            //充电电流高字节 
	                                       
	  buff = (unsigned int)(DC_ChargeTime);
	  mg.data[4] = buff;   //累计充电时间低字节  
	  mg.data[5] = buff>>8;  
	 
    buff = (unsigned int)(DC_ChargePower*10);
    mg.data[6] = buff;    //充电电量 
    mg.data[7] = buff>>8;                  
     
    mg.id= 0x1828D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name  : BMS_To_VCU_BatteryMsg1
//* period         : 100ms
//* Description    : BMS_1
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_To_VCU_BatteryMsg1(void) 
{
    struct can_msg mg;
    unsigned char tt=100;
    unsigned int buff;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    buff = (unsigned int)(BiggestFeedbackCurt);   //允许最大10S充电电流
    mg.data[0] = buff;
    mg.data[1] = buff>>8; 
    
    buff = (unsigned int)(BiggestFeedbackCurtContinuous); //允许最大持续充电电流
    mg.data[2] = buff;
    mg.data[3] = buff>>8;
    
    buff = (unsigned int)(BiggestDischargeCurt);	//允许最大10S放电电流
    mg.data[4] = buff;
    mg.data[5] = buff>>8;
    
    buff = (unsigned int)(BiggestDisCurtContinuous);	//允许最大持续放电电流
    mg.data[6] = buff;
    mg.data[7] = buff>>8;
      
    mg.id= 0x1422D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//*************************************************************************************
//* Function name  : BMS_To_VCU_BatteryMsg2
//* period         : 100ms
//* Description    : BMS_2
//* EntryParameter : None
//* ReturnValue    : None
//*************************************************************************************
void BMS_To_VCU_BatteryMsg2(void) 
{
    struct can_msg mg;
    unsigned char tt=100;
    unsigned int buff;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
	  
	  buff = (unsigned int)(g_systemVoltage*10);	      //动力电池总电压
	  mg.data[0] = buff;   //低字节 
	  mg.data[1] = buff>>8;//高字节    
	  
	  buff = (unsigned int)((g_systemCurrent+1000)*10);	//动力电池总电流
	  mg.data[2] = buff;   //低字节 
	  mg.data[3] = buff>>8;//高字节     
    
    if(Can_g_socValue>=0.995) 
        buff = 100*2;
    else 
        buff = (unsigned int)(Can_g_socValue*100*2);    //系统SOC
       
    mg.data[4] = buff;      //SOC低字节      
    mg.data[5] = buff>>8;      //SOC高字节
     
    mg.data[6] = g_highestTemperature;                   //最高温度  (温度本身-40偏移量+40=0)
    mg.data[7] = g_lowestTemperature;                    //最低温度
     
    mg.id= 0x1423D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//*************************************************************************************
//* Function name  : BMS_To_VCU_BatteryMsg3
//* period         : 100ms
//* Description    : BMS_3
//* EntryParameter : None
//* ReturnValue    : None
//*************************************************************************************
void BMS_To_VCU_BatteryMsg3(void) 
{
    struct can_msg mg;
    unsigned char tt=100;
    unsigned int buff;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
	  
	  buff = (unsigned int)(g_highestCellVoltage*1000); // 当前最高单体电池电压 0.001V
	  mg.data[0] = buff;   //低字节 
	  mg.data[1] = buff>>8;//高字节    
	  
	  buff = (unsigned int)(g_lowestCellVoltage*1000);	//当前最低单体电池电压 0.001V
	  mg.data[2] = buff;   //低字节 
	  mg.data[3] = buff>>8;//高字节     
    
    buff = (unsigned int)(Rp_Vpn_Value);   //正绝缘电阻单位1k
    mg.data[4] = buff;
    mg.data[5] = buff>>8;
    
    buff = (unsigned int)(Rn_Vpn_Value);   //负绝缘电阻单位1k
    mg.data[6] = buff;
    mg.data[7] = buff>>8;
     
    mg.id= 0x1424D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//*************************************************************************************
//* Function name  : BMS_To_VCU_BatteryMsg4
//* period         : 100ms 请求发送
//* Description    : BMS_4
//* EntryParameter : None
//* ReturnValue    : None
//*************************************************************************************
void BMS_To_VCU_BatteryMsg4(void)    // 
{
    struct can_msg mg;
    unsigned char tt=100;
    unsigned int buff;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
	  
	  buff = (unsigned int)(g_highestCellVoltage*1000); // 当前最高单体电池电压 0.001V
	  mg.data[0] = buff;   //低字节 
	  mg.data[1] = buff>>8;//高字节   
	   
	  buff = (unsigned int)(g_lowestCellVoltage*1000);	//当前最低单体电池电压 0.001V
	  mg.data[2] = buff;   //低字节 
	  mg.data[3] = buff>>8;//高字节   
	  
	  //mg.data[4] = highestCellVolNum;   //最高单体动力蓄电池电压所在总成号
	  mg.data[4] = 0x01;   //假设整个系统为一个总成
	  mg.data[5] = HighVolNum+1;   //+1是因为从0开始   最高单体动力蓄电池电压所在总成号内编号
	  
   // mg.data[6] = lowestCellVolNum;   //最低单体动力蓄电池电压所在总成号
    mg.data[6] = 0x01;// //假设整个系统为一个总成
    mg.data[7] = LowVolNum+1;   //最低单体动力蓄电池电压所在总成号内编号
     
    mg.id= 0x1813D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//*************************************************************************************
//* Function name  : BMS_To_VCU_BatteryMsg5
//* period         : 100ms 请求发送
//* Description    : BMS_5
//* EntryParameter : None
//* ReturnValue    : None
//*************************************************************************************
void BMS_To_VCU_BatteryMsg5(void)   
{
    struct can_msg mg;
    unsigned char tt=100;
    unsigned int buff;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
	  
  	   
	  mg.data[0] = g_highestTemperature-40+50;   //最高动力蓄电池温度
	  mg.data[1] = g_lowestTemperature-40+50;    //最低动力蓄电池温度 
	 
    //mg.data[2] = highestCellTempNum;   //最高动力蓄电池温度所在总成号
    mg.data[2] = 0x01;   //假设一个系统算为总成号1
	  mg.data[3] = HighTemNum+1;   //最高动力蓄电池温度所在总成号内编号
	  
    //mg.data[4] = lowestCellTempNum;   //最低动力蓄电池温度所在总成号
    mg.data[4] = 0x01;   //假设一个系统算为总成号1
    mg.data[5] = LowTemNum+1;   //最低动力蓄电池温度所在总成号内编号
    
    buff = (unsigned int)(CELL_TEMP_NUM);	//电池系统温度探针总数
	  mg.data[6] = buff;   //低字节 
	  mg.data[7] = buff>>8;//高字节    
    
    mg.id= 0x1814D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//*************************************************************************************
//* Function name  : BMS_To_VCU_BatteryMsg6
//* period         : 100ms 
//* Description    : BMS_6 BMS状态信息
//* EntryParameter : None
//* ReturnValue    : None
//*************************************************************************************
void BMS_To_VCU_BatteryMsg6(void)   
{
    struct can_msg mg;
    unsigned char tt=100;
    unsigned int buff;
    static unsigned char life=14;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
	  life--;
	  if(life < 0)
	      life = 14;
    status_group1.Bit.BMS_Life = life;
    	   
	  mg.data[0] = mg.data[1]^mg.data[2]^mg.data[3]^mg.data[4]^mg.data[5]^mg.data[6]^mg.data[7]; //chucksum
	  mg.data[1] = status_group1.byte;   
    mg.data[2] = status_group2.byte;   
	  mg.data[3] = status_group3.byte;  
	 
    mg.data[4] = plug_DC_Connect;   //预留
    mg.data[5] = CC2VOL;   //预留
    mg.data[6] = 0x00;   //预留
    mg.data[7] = 0x00;   //预留
     
    mg.id= 0x1881D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//*************************************************************************************
//* Function name  : BMS_To_VCU_BatteryMsg7
//* period         : 100ms 
//* Description    : BMS_7  电池组当前信息
//* EntryParameter : None
//* ReturnValue    : None
//*************************************************************************************
void BMS_To_VCU_BatteryMsg7(void)   
{
    struct can_msg mg;
    unsigned char tt=100;
    unsigned int buff;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
	  
  	   
	  mg.data[0] = Error_Group0.byte;//充电插座过温，火灾预警
	  mg.data[1] = Error_Group6.byte; 
    mg.data[2] = status_group4.byte;   
	  mg.data[3] = Error_Group1.byte;  
	 
    mg.data[4] = Error_Group2.byte;   
    mg.data[5] = Error_Group3.byte;   
    mg.data[6] = Error_Group4.byte;   
    mg.data[7] = Error_Group5.byte;   
     
    mg.id= 0x0C1FD0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}

//*************************************************************************************
//* Function name  : BMS_To_VCU_BatteryMsg8
//* period         : 100ms 
//* Description    : BMS_8  电池组当前信息
//* EntryParameter : None
//* ReturnValue    : None
//*************************************************************************************
void BMS_To_VCU_BatteryMsg8(void)   
{
    struct can_msg mg;
    unsigned char tt=100;
    unsigned int buff;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
	  
  	   
	  mg.data[0] = Error_Group7.byte;//
	  mg.data[1] = 0x00; 
    mg.data[2] = 0x00;   
	  mg.data[3] = 0x00;  
	 
    mg.data[4] = 0x00;   
    mg.data[5] = 0x00;   
    mg.data[6] = 0x00;   
    mg.data[7] = 0x00;   
     
    mg.id= 0x0C1ED0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}


void BMS_To_VCU_BatCellVolData(void)
{
	struct can_msg mg;
	unsigned int buff;
	unsigned char i;
	unsigned char tt=100;
	static U8 pack_cnt = 0;
	static U8 cell_cnt = 0;
	
	mg.RTR= FALSE;
	mg.len = 8;
	mg.prty = 0;
	mg.id= 0x1c15d0d2;
	
	mg.data[0] = cell_cnt;		//start at 0, 0~255
	mg.data[1] = pack_cnt + 1;	//start at 1, 1~255 
	
	mg.data[2] = (U8)(g_CellVoltage[pack_cnt][cell_cnt] & 0x00ff);
	mg.data[3] = (U8)((g_CellVoltage[pack_cnt][cell_cnt] >> 8) & 0x00ff);
	
	mg.data[4] = (U8)(g_CellVoltage[pack_cnt][cell_cnt+1] & 0x00ff);
	mg.data[5] = (U8)((g_CellVoltage[pack_cnt][cell_cnt+1] >> 8) & 0x00ff);
	
	mg.data[6] = (U8)(g_CellVoltage[pack_cnt][cell_cnt+2] & 0x00ff);
	mg.data[7] = (U8)((g_CellVoltage[pack_cnt][cell_cnt+2] >> 8) & 0x00ff);

	cell_cnt += 3;
	if(cell_cnt >= 36){
		cell_cnt = 0;
		pack_cnt++;
		if(pack_cnt >= BMU_NUMBER){
			pack_cnt = 0;
		}
	}
	
	while((!MSCAN0SendMsg(mg))&&(tt>0))
		tt--;
}

//******************************************************************************
//* Function name:   BMS_To_VCU_BatCellTempData
//* Description:     500ms 多帧数据
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_To_VCU_BatCellTempData(unsigned char group)
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char i;
    unsigned char tt=100;

    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;     
    mg.id= 0x1c64d0d2;

    mg.data[0] = group;
    mg.data[1] = group+1;//+1是因为从0开始
	
	for(i=2;i<8;i++)
	    mg.data[i]= g_cellTemperature[group][i-2]+10;//+10是因为偏移量为-50
  	     
	while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name:   BMS_To_VCU_BatCellTempData
//* Description:     500ms 多帧数据
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_VCU_WIFE(void)
{
  struct can_msg mg;
	unsigned int buff;
	unsigned char i;
	unsigned char tt=100;

  mg.RTR= FALSE;  
  mg.len = 8;
  mg.prty = 0;     
  
  mg.data[0] = 0x00;
  mg.data[1] = 0x00;//
  mg.data[2] = 0x00;
  mg.data[3] = VCU_ChgControl.byte;//降弓到位等信号
  mg.data[4] = 0x00;
  mg.data[5] = 0x00;//
  mg.data[6] = 0x00;
  mg.data[7] = WifeLife;//Life
	

  mg.id= 0x0c0217a7;
  	     
	while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}            
            
//*************************************************************************************
//*************************************************************************************
//*****************************以下是BMS给车载充电机发消息子程序***********************
//*************************************************************************************
//*************************************************************************************

//******************************************************************************
//* Function name:   bmsToPcTestCar
//* Description:     进入调试模式
//* EntryParameter : number
//* ReturnValue    : None
//******************************************************************************
void bmsToPcTestCar(void)
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    	
	  mg.data[0]= 0xA0; 
  	mg.data[1]= 0;

	  mg.data[2]= stateCode; 
	  mg.data[3]= bmsSelfcheckCounter; 
	  
	  buff = (unsigned int)(g_highVoltageV1*50);
	  mg.data[4]= (unsigned char)buff;//电池组当前电压低字节
	  mg.data[5]= buff>>8;//电池组当前电压高字节 
 
	  buff = (unsigned int)(g_highVoltageV2*50);
	  mg.data[6]= (unsigned char)buff;//电池组当前电压低字节 
	  mg.data[7]= buff>>8;//电池组当前电压高字节 

	  mg.id= 0x000c0453; 
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--;
}


void bmsToPcTestCar1(void)
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    	
	  buff = (unsigned int)(g_highVoltageV3*50);
	  mg.data[0]= (unsigned char)buff;//电池组当前电压低字节 
	  mg.data[1]= buff>>8;//电池组当前电压高字节 

	  buff = (unsigned int)(g_highVoltageV4*50);
	  mg.data[2]= (unsigned char)buff;//电池组当前电压低字节
	  mg.data[3]= buff>>8;//电池组当前电压高字节 

	  buff = (unsigned int)(g_highVoltageV5*50);
	  mg.data[4]= (unsigned char)buff;//电池组当前电压低字节
	  mg.data[5]= buff>>8;//电池组当前电压高字节 
	  
	  buff = (unsigned int)(g_highVoltageV6*50);
	  mg.data[6]= (unsigned char)buff;//电池组当前电压低字节
    mg.data[7]= buff>>8;//电池组当前电压高字节 

	  mg.id= 0x000c0454; 
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--;
}
//***********************************************************************
//************************************************************************
//*************************the end*************************************
//************************************************************************
