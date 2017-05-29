//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : S133
//* File Name          : VehicleCAN.h
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2015.7.13
//* Description        : ���ļ��Ƕ�CAN���͵ı��Ľ���BIT����
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
typedef union{
	unsigned char byte;
	struct {
	
	  byte F_Current_Sensor:1;  //bit0
    byte F_Inner_Communiction:1;	  //bit1
		byte F_Battery_Under_Voltage:3;  //bit2~bit4
		byte F_Battery_Over_Voltage:3;   //bit5~bit7   
		        		       
	}Bit;
}FAULT_GROUP1;    
//extern FAULT_GROUP1  Fault_Group1;

typedef union{
	unsigned char byte;
	struct {
	
	  byte F_Charge_Relay_Stick:1;
	  byte F_Temp_Sensor:1;
    byte F_DisCharge_Over_Current:3;
    byte F_Charge_Over_Current:3;  
      
	}Bit;
}FAULT_GROUP2;    
//extern FAULT_GROUP2  Fault_Group2;

typedef union{
	unsigned char byte;
	struct {
	
	  byte F_SOC_Low:2;
	  byte F_Cell_Over_Voltage:3;
	  byte F_Cell_Under_Voltage:3;
	  	 
	}Bit;
}FAULT_GROUP3;    
//extern FAULT_GROUP3  Fault_Group3;

typedef union{
	unsigned char byte;
	struct {
	  
  	byte State_N_Relay:1;
    byte F_Balance:1;
  	byte F_Temp_Unbalance:3;
    byte F_Cell_Unbalance:3;	
    
	}Bit;
}FAULT_GROUP4;    
//extern FAULT_GROUP4  Fault_Group4;

typedef union{
	unsigned char byte;
	struct {
    
	  byte Reserve1_Bit:1;
	  byte F_Communication_With_Charger:1;
	  byte F_Under_Temp:3;
	  byte F_Over_Temp:3;    

	}Bit;
}FAULT_GROUP5;    
//extern FAULT_GROUP5  Fault_Group5;

typedef union{
	unsigned char byte;
	struct {
    
    byte State_Balance:1;    
  	byte F_N_Relay_Break:1;
    byte F_N_Relay_Stick:1;	 
	  byte State_Charge_Plug:1;
	  byte State_Charge:1;
	  byte State_Fault:1;
	  byte State_Self_Check:2;
	  

	}Bit;
}FAULT_GROUP6;    
//extern FAULT_GROUP6  Fault_Group6;

typedef union{
	unsigned char byte;
	struct {
    byte Reserve2_Bit:1;
    byte Reserve3_Bit:1;
    byte Reserve4_Bit:1;
    byte Reserve5_Bit:1;
    byte Reserve6_Bit:1;
  	byte F_SOC_High:3;
	}Bit;
}FAULT_GROUP7;    
//extern FAULT_GROUP7  Fault_Group7;


typedef union{
	unsigned char byte;
	struct {
		byte F1_rechargeOC:1;       
		byte F1_dischargeOC:1;              
		byte F1_batteryOV:1;              
		byte F1_batteryUV:1;
		byte F1_isolation:1;
		byte F1_SOCLow:1;
		byte F1_cellUV:1;
		byte F1_cellOV:1;
	}Bit;
}CAN08F0BYTE0;    
extern CAN08F0BYTE0      Can08f0Byte0;

typedef union{
	unsigned char byte;
	struct {
		byte F1_underTemp:1;       
		byte F1_overTemp:1;              
		byte CellUnbalance:1;              
		byte RepairsSwitch:1;
		byte tempSensor:1;
		byte currentSensor:1;
		byte voltageSensor:1;
		byte flag7:1;
	}Bit;
}CAN08F0BYTE1;    
extern CAN08F0BYTE1      Can08f0Byte1;

typedef union{
	unsigned char byte;
	struct {
		byte F2_rechargeOC:1;       
		byte F2_dischargeOC:1;              
		byte F2_batteryOV:1;              
		byte F2_batteryUV:1;
		byte F2_isolation:1;
		byte F2_SOCLow:1;
		byte F2_cellUV:1;
		byte F2_cellOV:1;
	}Bit;
}CAN08F0BYTE2;    
extern CAN08F0BYTE2      Can08f0Byte2;

typedef union{
	unsigned char byte;
	struct {
		byte F2_underTemp:1;       
		byte F2_overTemp:1;              
		byte flag2:1;              
		byte flag3:1;
		byte flag4:1;
		byte flag5:1;
		byte flag6:1;
		byte flag7:1;
	}Bit;
}CAN08F0BYTE3;    
extern CAN08F0BYTE3      Can08f0Byte3;

typedef union{
	unsigned char byte;
	struct {
		byte F3_rechargeOC:1; //����������������䡢���䶼�������־λ      
		byte F3_dischargeOC:1;//�ŵ����              
		byte F3_batteryOV:1;  //��ذ���ѹ              
		byte F3_batteryUV:1;  //��ذ�Ƿѹ       
		byte flag4:1;
		byte F3_SOCLow:1;    //SOC�� 
		byte F3_cellUV:1;     //����Ƿѹ
		byte F3_cellOV:1;     //�����ѹ
	}Bit;
}CAN08F0BYTE4;    
extern CAN08F0BYTE4      Can08f0Byte4;

typedef union{
	unsigned char byte;
	struct {     
		byte F3_overTemp:1;              
		byte F3_charge:1;              
		byte F3_isolation:1;
		byte F3_mainRelayError:1;  //���̵������ϡ�������������ճ���Ͷ�·����
		byte F3_preRelayError:1;   //Ԥ��̵�������  //����ճ���Ͷ�·����
		byte F3_innerComm:1;  //�ڲ�ͨ�Ź���
		byte flag6:1;
		byte flag7:1;
	}Bit;
}CAN08F0BYTE5;    
extern CAN08F0BYTE5      Can08f0Byte5;

typedef union{
	unsigned char byte;
	struct {     
		byte st_chargerconnect:2;              
		byte flag2:1;              
		byte flag3:1;
		byte st_po_batteryRelay:1;  
		byte st_chargeRelay:1;
		byte st_ne_batteryRelay:1;
		byte st_prechargeRelay:1;
	}Bit;
}CAN10F2BYTE0;    
extern CAN10F2BYTE0      Can10f2Byte0;

//******************************************************************************
//* �����������ⲿ�ı���
//******************************************************************************

extern FAULT_GROUP1  Fault_Group1;
extern FAULT_GROUP2  Fault_Group2;
extern FAULT_GROUP3  Fault_Group3;
extern FAULT_GROUP4  Fault_Group4;
extern FAULT_GROUP5  Fault_Group5;
extern FAULT_GROUP6  Fault_Group6;
extern FAULT_GROUP7  Fault_Group7;



extern float BiggestFeedbackPower;  //���������:Byte3
extern float BiggestDischargePower; //���ŵ繦��:Byte5
extern float biggestFeedbackCurrent;  //��������������
extern float biggestDischargeCurrent; //�������ŵ����


extern unsigned char Box_Num_g_highestCellVoltage; //��ߵ����������
extern unsigned char Box_Posi_g_highestCellVoltage;//��ߵ����������λ��

extern unsigned char Box_Num_g_lowestCellVoltage;  //��͵����������
extern unsigned char Box_Posi_g_lowestCellVoltage; //��͵����������λ��

extern unsigned char Box_Num_g_highestTemperature;  //����¶��������
extern unsigned char Module_Num_g_highestTemperature;//����¶�����ģ��

extern unsigned char State_Box_Online;               //�����������
extern unsigned char Box_Quantity;                   //��������

extern unsigned char Box_Num_g_lowestTemperature;    //����¶��������
extern unsigned char Module_Posi_g_lowestTemperature;//����¶�����ģ��

extern unsigned int Charge_Times;                    //������


extern unsigned char  SwithState;   //��������:Byte6
extern unsigned int stateCode; //״̬������
extern unsigned char pusleLimitSec;     //��������������ʱ��
extern unsigned char St_heatManage;//��������ȹ���״̬��0û������1���ȹ����У�2cooling


//******************************************************************************
//* �����������ⲿ�ĺ���
//******************************************************************************
extern void BMS_To_VCU_BatMsg1(void); 
extern void BMS_To_VCU_BatMsg2(void); 
extern void BMS_To_VCU_Fault(void);
extern void BMS_To_VCU_CellExtremumMsg1(void);
extern void BMS_To_VCU_CellExtremumMsg2(void);
extern void BMS_To_VCU_PackMsg(void);
extern void BMS_To_VCU_ChargeMsg(void); 
//BMS_To_VCU_CellVoltage(unsigned char number);
//BMS_To_VCU_CellTem(unsigned char number);


extern void bmsToPcTestCar(void);
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////end/////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////