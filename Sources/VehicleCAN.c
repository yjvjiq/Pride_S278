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
//* Description        : ���ļ�������Ŀͨ��Э�鷢����صı��ĵ�������������
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

unsigned int Charge_Times;     //������
unsigned long int chargeAH ;   //ʵ�ʳ���ܰ�ʱ
unsigned long int dischargeAH ;//ʵ�ʷŵ��ܰ�ʱ
float DC_ChargePower=0;//�����������������


/*futon Logistics use*/

float g_SOH=1;                                //SOH
                                             
float BiggestDischargeCurt = 0;              //���ŵ����
float BiggestFeedbackCurt  = 0;              //����������
float BiggestDisCurtContinuous = 0;          //�������ŵ����
float BiggestFeedbackCurtContinuous = 0;     //��������������

float BiggestDischargePower = 0;              //���ŵ繦��
float BiggestFeedbackPower  = 0;              //����������
float BiggestDisPowerContinuous = 0;          //�������ŵ繦��
float BiggestFeedbackContinuous = 0;          //��������������

unsigned int chargeRemainderTime = 0;          //ʣ����ʱ��


float VehicleSpeed = 0;                         //�����źſ��                           
unsigned char WifeLife=0;  //wife�ź������ź� ����ܵ繭����ڿ��CAN�ϣ���Ҫת��������CAN
                                                
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
//*****************************������BMS��VCU����Ϣ�ӳ���******************************
//*************************************************************************************
//*************************************************************************************
//*************************************************************************************
//* Function name  : BMS_To_VCU_BasicMsg0
//* period         : 1000ms
//* Description    : ����������Ϣ3
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
	  
	  buff = (unsigned int)(SYS_KWH*10); //�����ϵͳ������138.2kwh=240*3.2*180
	  mg.data[0] = buff;   //���ֽ� 
	  mg.data[1] = buff>>8;//���ֽ�    
	  
	  buff = CELL_NUM;	//��ؽ���
	  mg.data[2] = buff;   //���ֽ� 
	  mg.data[3] = buff>>8;//���ֽ� 
	  	  
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
	  
	  buff = (unsigned int)(HIGHEST_ALLOWED_CHARGE_V*10); //����������ܵ�ѹ
	  mg.data[0] = buff;   //���ֽ� 
	  mg.data[1] = buff>>8;//���ֽ�    
	  
	  buff = (unsigned int)(SYS_NOMINAL_V*10);	//������Ƶ�ѹƽ̨
	  mg.data[2] = buff;   //���ֽ� 
	  mg.data[3] = buff>>8;//���ֽ� 
	  	  
	  buff = (unsigned int)(HIGHEST_ALLOWED_CHARGE_T+40); //����������¶�  
	  mg.data[4] = buff;   //���ֽ�    
	  
	  buff = (unsigned int)(LOWEST_ALLOWED_WORK_T+40); //����������¶�
	  mg.data[5] = buff;   //���ֽ�    
	 
    mg.data[6] = 0x00;                  //Ԥ��
    mg.data[7] = BMU_NUMBER;                  //LECU���� λBMU����
     
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
	  
	  buff = (unsigned int)(HIGHEST_CELL_VOL*1000); // ��������߱�����ѹ
	  mg.data[0] = buff;   //���ֽ� 
	  mg.data[1] = buff>>8;//���ֽ�    
	  
	  buff = (unsigned int)(LOWEST_CELL_VOL*1000);	//��������ͱ�����ѹ
	  mg.data[2] = buff;   //���ֽ� 
	  mg.data[3] = buff>>8;//���ֽ� 
	  	  
	  buff = (unsigned int)(SYS_NOMINAL_AH*10); //������ư�ʱ��
	  mg.data[4] = buff;   //���ֽ� 
	  mg.data[5] = buff>>8;   //���ֽ�     
	  
    mg.data[6] = g_SOH*100; //SOHֵ   
    mg.data[7] = CELL_TYPE|(COOL_TYPE<<4);                  //�������������� 3 ��Ȼ��ȴ
     
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
	  mg.data[0]= (unsigned char)buff;         // ����ѹ���ֽ�
    mg.data[1]= buff>>8;                     // ����ѹ���ֽ�
    
	  buff = (unsigned int)((g_systemCurrent+400)*10);
  	mg.data[2]= (unsigned char)buff;//���������ֽ� 
    mg.data[3]= buff>>8;            //���������ֽ� 
	                                       
	  buff = (unsigned int)(DC_ChargeTime);
	  mg.data[4] = buff;   //�ۼƳ��ʱ����ֽ�  
	  mg.data[5] = buff>>8;  
	 
    buff = (unsigned int)(DC_ChargePower*10);
    mg.data[6] = buff;    //������ 
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
    
    buff = (unsigned int)(BiggestFeedbackCurt);   //�������10S������
    mg.data[0] = buff;
    mg.data[1] = buff>>8; 
    
    buff = (unsigned int)(BiggestFeedbackCurtContinuous); //����������������
    mg.data[2] = buff;
    mg.data[3] = buff>>8;
    
    buff = (unsigned int)(BiggestDischargeCurt);	//�������10S�ŵ����
    mg.data[4] = buff;
    mg.data[5] = buff>>8;
    
    buff = (unsigned int)(BiggestDisCurtContinuous);	//�����������ŵ����
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
	  
	  buff = (unsigned int)(g_systemVoltage*10);	      //��������ܵ�ѹ
	  mg.data[0] = buff;   //���ֽ� 
	  mg.data[1] = buff>>8;//���ֽ�    
	  
	  buff = (unsigned int)((g_systemCurrent+1000)*10);	//��������ܵ���
	  mg.data[2] = buff;   //���ֽ� 
	  mg.data[3] = buff>>8;//���ֽ�     
    
    if(Can_g_socValue>=0.995) 
        buff = 100*2;
    else 
        buff = (unsigned int)(Can_g_socValue*100*2);    //ϵͳSOC
       
    mg.data[4] = buff;      //SOC���ֽ�      
    mg.data[5] = buff>>8;      //SOC���ֽ�
     
    mg.data[6] = g_highestTemperature;                   //����¶�  (�¶ȱ���-40ƫ����+40=0)
    mg.data[7] = g_lowestTemperature;                    //����¶�
     
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
	  
	  buff = (unsigned int)(g_highestCellVoltage*1000); // ��ǰ��ߵ����ص�ѹ 0.001V
	  mg.data[0] = buff;   //���ֽ� 
	  mg.data[1] = buff>>8;//���ֽ�    
	  
	  buff = (unsigned int)(g_lowestCellVoltage*1000);	//��ǰ��͵����ص�ѹ 0.001V
	  mg.data[2] = buff;   //���ֽ� 
	  mg.data[3] = buff>>8;//���ֽ�     
    
    buff = (unsigned int)(Rp_Vpn_Value);   //����Ե���赥λ1k
    mg.data[4] = buff;
    mg.data[5] = buff>>8;
    
    buff = (unsigned int)(Rn_Vpn_Value);   //����Ե���赥λ1k
    mg.data[6] = buff;
    mg.data[7] = buff>>8;
     
    mg.id= 0x1424D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//*************************************************************************************
//* Function name  : BMS_To_VCU_BatteryMsg4
//* period         : 100ms ������
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
	  
	  buff = (unsigned int)(g_highestCellVoltage*1000); // ��ǰ��ߵ����ص�ѹ 0.001V
	  mg.data[0] = buff;   //���ֽ� 
	  mg.data[1] = buff>>8;//���ֽ�   
	   
	  buff = (unsigned int)(g_lowestCellVoltage*1000);	//��ǰ��͵����ص�ѹ 0.001V
	  mg.data[2] = buff;   //���ֽ� 
	  mg.data[3] = buff>>8;//���ֽ�   
	  
	  //mg.data[4] = highestCellVolNum;   //��ߵ��嶯�����ص�ѹ�����ܳɺ�
	  mg.data[4] = 0x01;   //��������ϵͳΪһ���ܳ�
	  mg.data[5] = HighVolNum+1;   //+1����Ϊ��0��ʼ   ��ߵ��嶯�����ص�ѹ�����ܳɺ��ڱ��
	  
   // mg.data[6] = lowestCellVolNum;   //��͵��嶯�����ص�ѹ�����ܳɺ�
    mg.data[6] = 0x01;// //��������ϵͳΪһ���ܳ�
    mg.data[7] = LowVolNum+1;   //��͵��嶯�����ص�ѹ�����ܳɺ��ڱ��
     
    mg.id= 0x1813D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//*************************************************************************************
//* Function name  : BMS_To_VCU_BatteryMsg5
//* period         : 100ms ������
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
	  
  	   
	  mg.data[0] = g_highestTemperature-40+50;   //��߶��������¶�
	  mg.data[1] = g_lowestTemperature-40+50;    //��Ͷ��������¶� 
	 
    //mg.data[2] = highestCellTempNum;   //��߶��������¶������ܳɺ�
    mg.data[2] = 0x01;   //����һ��ϵͳ��Ϊ�ܳɺ�1
	  mg.data[3] = HighTemNum+1;   //��߶��������¶������ܳɺ��ڱ��
	  
    //mg.data[4] = lowestCellTempNum;   //��Ͷ��������¶������ܳɺ�
    mg.data[4] = 0x01;   //����һ��ϵͳ��Ϊ�ܳɺ�1
    mg.data[5] = LowTemNum+1;   //��Ͷ��������¶������ܳɺ��ڱ��
    
    buff = (unsigned int)(CELL_TEMP_NUM);	//���ϵͳ�¶�̽������
	  mg.data[6] = buff;   //���ֽ� 
	  mg.data[7] = buff>>8;//���ֽ�    
    
    mg.id= 0x1814D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//*************************************************************************************
//* Function name  : BMS_To_VCU_BatteryMsg6
//* period         : 100ms 
//* Description    : BMS_6 BMS״̬��Ϣ
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
	 
    mg.data[4] = plug_DC_Connect;   //Ԥ��
    mg.data[5] = CC2VOL;   //Ԥ��
    mg.data[6] = 0x00;   //Ԥ��
    mg.data[7] = 0x00;   //Ԥ��
     
    mg.id= 0x1881D0D2;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//*************************************************************************************
//* Function name  : BMS_To_VCU_BatteryMsg7
//* period         : 100ms 
//* Description    : BMS_7  ����鵱ǰ��Ϣ
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
	  
  	   
	  mg.data[0] = Error_Group0.byte;//���������£�����Ԥ��
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
//* Description    : BMS_8  ����鵱ǰ��Ϣ
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
//* Description:     500ms ��֡����
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
    mg.data[1] = group+1;//+1����Ϊ��0��ʼ
	
	for(i=2;i<8;i++)
	    mg.data[i]= g_cellTemperature[group][i-2]+10;//+10����Ϊƫ����Ϊ-50
  	     
	while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name:   BMS_To_VCU_BatCellTempData
//* Description:     500ms ��֡����
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
  mg.data[3] = VCU_ChgControl.byte;//������λ���ź�
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
//*****************************������BMS�����س�������Ϣ�ӳ���***********************
//*************************************************************************************
//*************************************************************************************

//******************************************************************************
//* Function name:   bmsToPcTestCar
//* Description:     �������ģʽ
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
	  mg.data[4]= (unsigned char)buff;//����鵱ǰ��ѹ���ֽ�
	  mg.data[5]= buff>>8;//����鵱ǰ��ѹ���ֽ� 
 
	  buff = (unsigned int)(g_highVoltageV2*50);
	  mg.data[6]= (unsigned char)buff;//����鵱ǰ��ѹ���ֽ� 
	  mg.data[7]= buff>>8;//����鵱ǰ��ѹ���ֽ� 

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
	  mg.data[0]= (unsigned char)buff;//����鵱ǰ��ѹ���ֽ� 
	  mg.data[1]= buff>>8;//����鵱ǰ��ѹ���ֽ� 

	  buff = (unsigned int)(g_highVoltageV4*50);
	  mg.data[2]= (unsigned char)buff;//����鵱ǰ��ѹ���ֽ�
	  mg.data[3]= buff>>8;//����鵱ǰ��ѹ���ֽ� 

	  buff = (unsigned int)(g_highVoltageV5*50);
	  mg.data[4]= (unsigned char)buff;//����鵱ǰ��ѹ���ֽ�
	  mg.data[5]= buff>>8;//����鵱ǰ��ѹ���ֽ� 
	  
	  buff = (unsigned int)(g_highVoltageV6*50);
	  mg.data[6]= (unsigned char)buff;//����鵱ǰ��ѹ���ֽ�
    mg.data[7]= buff>>8;//����鵱ǰ��ѹ���ֽ� 

	  mg.id= 0x000c0454; 
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--;
}
//***********************************************************************
//************************************************************************
//*************************the end*************************************
//************************************************************************
