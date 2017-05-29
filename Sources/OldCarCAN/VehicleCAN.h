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


typedef union{
	unsigned char byte;
	struct {
	
	  byte F_Charge_Relay_Stick:1;
	  byte F_Temp_Sensor:1;
    byte F_DisCharge_Over_Current:3;
    byte F_Charge_Over_Current:3;  
      
	}Bit;
}FAULT_GROUP2;    


typedef union{
	unsigned char byte;
	struct {
	
	  byte F_SOC_Low:2;
	  byte F_Cell_Over_Voltage:3;
	  byte F_Cell_Under_Voltage:3;
	  	 
	}Bit;
}FAULT_GROUP3;    


typedef union{
	unsigned char byte;
	struct {
	  
  	byte State_N_Relay:1;
    byte F_Balance:1;
  	byte F_Temp_Unbalance:3;
    byte F_Cell_Unbalance:3;	
    
	}Bit;
}FAULT_GROUP4;    


typedef union{
	unsigned char byte;
	struct {
    
	  byte Reserve1_Bit:1;
	  byte F_Communication_With_Charger:1;
	  byte F_Under_Temp:3;
	  byte F_Over_Temp:3;    

	}Bit;
}FAULT_GROUP6;    

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
}FAULT_GROUP7;    


typedef union{
	unsigned char byte;
	struct {
    byte Reserve1_Bit:1;
    byte Reserve2_Bit:1;
    byte SelfCount:3;
  	byte F_SOC_High:3;
	}Bit;
}FAULT_GROUP8;    

//******************************************************************************
//* �����������ⲿ�ı���
//******************************************************************************
extern FAULT_GROUP1  Fault_Group1;
extern FAULT_GROUP2  Fault_Group2;
extern FAULT_GROUP3  Fault_Group3;
extern FAULT_GROUP4  Fault_Group4;
extern FAULT_GROUP6  Fault_Group6;
extern FAULT_GROUP7  Fault_Group7;
extern FAULT_GROUP8  Fault_Group8;



extern float BiggestFeedbackPower;  //���������:Byte3
extern float BiggestDischargePower; //���ŵ繦��:Byte5
extern float biggestFeedbackCurrent;  //��������������
extern float biggestDischargeCurrent; //�������ŵ����
extern float biggestDisCurtContinuous; //�������ŵ����5min
extern float biggestfeedbackContinuous;//������������5min


extern unsigned char Box_Num_g_highestCellVoltage; //��ߵ����������
extern unsigned char Box_Posi_g_highestCellVoltage;//��ߵ����������λ��

extern unsigned char Box_Num_g_lowestCellVoltage;  //��͵����������
extern unsigned char Box_Posi_g_lowestCellVoltage; //��͵����������λ��

extern unsigned char Box_Num_g_highestTemperature;  //����¶��������
extern unsigned char Module_Posi_g_highestTemperature;//����¶�����ģ��

extern unsigned char State_Box_Online;               //�����������
extern unsigned char Box_Quantity;                   //��������

extern unsigned char Box_Num_g_lowestTemperature;    //����¶��������
extern unsigned char Module_Posi_g_lowestTemperature;//����¶�����ģ��

extern unsigned int Charge_Times;                    //������


extern unsigned char  SwithState;   //��������:Byte6
extern unsigned char stateCode; //״̬������
extern unsigned char pusleLimitSec;     //��������������ʱ��
extern unsigned char St_heatManage;//��������ȹ���״̬��0û������1���ȹ����У�2cooling

extern unsigned long int chargeAH ;   //ʵ�ʳ���ܰ�ʱ
extern unsigned long int dischargeAH ;//ʵ�ʷŵ��ܰ�ʱ

//******************************************************************************
//* �����������ⲿ�ĺ���
//******************************************************************************
extern void BMS_To_VCU_BatMsg1(void); 
extern void BMS_To_VCU_BatMsg2(void); 
extern void BMS_To_VCU_Fault(void);
extern void BMS_To_VCU_CellExtremumMsg1(void);
extern void BMS_To_VCU_CellExtremumMsg2(void);
extern void BMS_To_VCU_PackMsg(void);  //TO ���أ�CAN0);
extern void BMS_To_VCU_ChargeMsg(void); 
extern void BMS_To_VCU_CellVoltage(unsigned char number);
extern void BMS_To_VCU_CellTem(unsigned char number);




extern void bmsToPcTestCar(void);
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////end/////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////