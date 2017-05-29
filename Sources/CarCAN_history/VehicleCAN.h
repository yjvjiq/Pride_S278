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
		byte B1_CellHigh:1;       
		byte B2_CellLow:1;              
		byte B3_SOCHigh:1;              
		byte B4_SOCLow:1;
		byte B5_ChaOverCurt:1;
		byte B6_DisChaOverCurt:1;
		byte B7_CellTemOver:1;
		byte B8_CellError:1;
	}Bit;
}CAN1818BYTE6;    
extern CAN1818BYTE6      Can1818Byte6;

typedef union{
	unsigned char byte;
	struct {
		byte B1_VolHigh:1;       
		byte B2_VolLow:1;              
		byte B3_BalanceAction:1;              
		byte B4_TemUnbalance:1;
		byte B57_ErrortLevel:3;
		byte B8_flag8:1;
	}Bit;
}CAN1818BYTE7;    
extern CAN1818BYTE7      Can1818Byte7;

typedef union{
	unsigned char byte;
	struct {
		byte B16_flag:6;       
		byte B7_RelayState:1;//�̵���״̬,0�Ͽ�,1�պ�
		byte B8_WorkState:1;//0:�ŵ磻1:���
	}Bit;
}CAN1818BYTE8;    
extern CAN1818BYTE8      Can1818Byte8;


typedef union{
	unsigned char byte;
	struct {
		byte B1_Num1ConnectError:1;      
		byte B2_Num2ConnectError:1;
		byte B3_Num3ConnectError:1;
	  byte B4_Num4ConnectError:1;
	  byte B5_Num5ConnectError:1;
	  byte B6_Num6ConnectError:1;
	  byte B78_flag:2;	
	}Bit;
}CAN181CBYTE3;    
extern CAN181CBYTE3       Can181cByte3;


typedef union{
	unsigned char byte;
	struct {
	  byte B14_flag:4;
	  byte B57_ErrorDone:3;
	  byte B8_ConnectState:1;	
	}Bit;
}CAN181CBYTE5;    
extern CAN181CBYTE5       Can181cByte5;


typedef union{
	unsigned char byte;
	struct {
	  byte B1_InsulationError:1;
	  byte B2_HighVolConError:1;
	  byte B3_WatchDog:1;
	  byte B4_ChanerContr:1;
	  byte B57_Battertype:3;
	  byte B8_ICdo:1; 	
	}Bit;
}CANF100BYTE3;    
extern CANF100BYTE3       Canf100Byte3;


/////////////////////////////////////////////////////////////////////////////////
extern float BiggestFeedbackPower;  //���������:Byte3
extern float BiggestDischargePower; //���ŵ繦��:Byte5
extern float biggestFeedbackCurrent;  //��������������
extern float biggestDischargeCurrent; //�������ŵ����
extern unsigned char  SwithState;   //��������:Byte6
extern unsigned int stateCode; //״̬������
extern unsigned char pusleLimitSec;     //��������������ʱ��
extern unsigned char St_heatManage;//��������ȹ���״̬��0û������1���ȹ����У�2cooling

extern unsigned char LCellBoxNum;//��͵�ѹ��Ӧ�����
extern unsigned char LCellLocation;//��͵����ѹ����λ��
extern unsigned char HCellBoxNum;//��ߵ�ѹ��Ӧ�����
extern unsigned char HCellLocation;//��ߵ����ѹ����λ��

extern unsigned char LTemBoxNum;//����¶ȶ�Ӧ�����
extern unsigned char LTemLocation;//����¶�����λ��
extern unsigned char HTemBoxNum;//����¶ȶ�Ӧ�����
extern unsigned char HTemLocation;//����¶�����λ��

extern unsigned char Box1HTem;
extern unsigned char Box2HTem;
extern unsigned char Box3HTem;
extern unsigned char Box4HTem;
extern unsigned char Box5HTem;
extern unsigned char Box6HTem;
extern unsigned char Box1LTem;
extern unsigned char Box2LTem;
extern unsigned char Box3LTem;
extern unsigned char Box4LTem;
extern unsigned char Box5LTem;
extern unsigned char Box6LTem;

extern unsigned char ChaStartH;
extern unsigned char ChaStartM;
extern unsigned char ChaStartS;
extern unsigned char ChaOverH;
extern unsigned char ChaOverM;
extern unsigned char ChaOverS;
extern float ChangerVol;
extern float ChangerCurt;

extern void BMS1_transmit1818(void); 
extern void BMS2_transmit1819(void);
extern void BMS3_transmit181A(void);
extern void BMS4_transmit181B(void);
extern void BMS5_transmit181C(void);
extern void BMS6_transmit181D(void);
extern void BMS7_transmit18F2(void);
extern void BMS_transmitFF2A(void);
extern void BMS_transmitFF2C(void);
extern void BMS_transmitFF2D(void);
extern void BMS_transmitFF2E(void);
extern void BMS_transmitFF30(void);
extern void BMS_transmitF100(void);
extern void BMS_transmitFF33(void);
extern void BMS_transmitFF34(void);
extern void BMS_transmitFF50(void);
extern void BMS_transmitFF51(void);     
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////end/////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////