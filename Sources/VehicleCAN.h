//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : fotun_Logistics
//* File Name          : VehicleCAN.h
//* Author             : WsM
//* Version            : V1.0.0
//* Start Date         : 2016.7.8
//* Description        : ���ļ�������Ŀͨ��Э�鷢����صı��ĵ�������������
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

//////////////////////VCU���͵�//////////////////////////
typedef union{
	unsigned char byte;
	struct {
	
	  byte flag:5;
    byte Finish_PreChg:1;	  //bit1   VCUԤ������
		byte flag1:2;              //bit4~7     ���̵���״̬
	}Bit;
}VCU_REQUEST;



typedef union{
	unsigned char byte;
	struct {
	
	  byte CellVolReq:1;    //bit0   ��ذ����ڵ㵥���ѹ
    byte CellTempReq:1;	  //bit1   ��ذ����ڵ��¶�
		byte PackMessageReq:1; //bit2  ��ذ���Ϣ  
		byte flag:5;              //bit3~7    Ԥ��
	}Bit;
}VCU_CELLREQUEST;

typedef union{
	unsigned char byte;
	struct {
	
	  byte VCULife:4;    //bit0~bit3   VCULife
    byte PowerOnOffReq:2;	  //bit4~bit5  ��ѹ���µ�ָ��
		byte flag:2;              //bit6~7    Ԥ��
	}Bit;
}VCU_CONTROL;


typedef union{
	unsigned char byte;
	struct {
	
	  byte flag1:2;  //bit0~1    Ԥ��
	  byte Charge_Switch:1;  //bit2   ��翪��
	  byte downC_Switch:1;   //bit3   ��������
	  byte flag2:1;  //bit4    Ԥ��
	  byte downC_OK:1;   //bit5   ������λ
		byte flag3:2;    //bit6~7    Ԥ��
	}Bit;
}VCU_CHGCONTROL;


typedef union{
	unsigned char byte;
	struct {
	
	  byte flag1:2;  //bit0~1    Ԥ��
	  byte Parking_Brake:2;  //bit2~3   פ���ź�
		byte flag2:4;    //bit6~7    Ԥ��
	}Bit;
}VCU_PARKINGBRAKE;
//////////////////////VCU���͵�//////////////////////////



 ////////////////////// BMS ���͵�״̬��Ϣ//////////////////////////
typedef union{
	unsigned char byte;
	struct {
	
	  byte BMS_Life:4;    //bit0~bit3  life
	  byte St_BMS :4;    //bit4~bit7   BMS��ǰ״̬
     		       
	}Bit;
}STATUS_GROUP1;

typedef union{
	unsigned char byte;
	struct {
	
	  byte BMS_PowerOff_Req:2;  //bit0~bit1     BMS��ѹ�µ����� ??????�Զ����
	  byte St_BMS_Balance:2;    //bit2~bit3  ��ǰ����״̬
	  byte St_Ele_Relay :2;     //bit4~bit5  �ܵ���Ӵ�������״̬
	  byte St_CHG_Connect :2;   //bit6~bit7  �������״̬
     		       
	}Bit;
}STATUS_GROUP2;

typedef union{
	unsigned char byte;
	struct {
	
	  byte St_CHG_Mode :2;   //bit0~bit1   ���ģʽ 
	  byte St_Charge:2;        //bit2~bit3  ���״̬
	  byte Fault_Level :2;    //bit4~bit5  ���ϵȼ�
	  byte St_N_Relay :1;    //bit6  �����̵���״̬����������������������
	  byte St_P_Relay :1;    //bit7  �����̵���״̬���������������������Զ����
     		       
	}Bit;
}STATUS_GROUP3;      

typedef union{
	unsigned char byte;
	struct {
	  byte St_BMS_System :1;    //bit0  BMSϵͳ״̬λ
	  byte St_BMU_System :1;    //bit1  BMUϵͳ״̬λ
	  byte St_LECU_System :2;    //bit2~bit3  BMUϵͳ״̬λ
	  byte St_Bat_Fault:2;    //bit4~bit5  ��������״̬
	  byte Mode_BMS_Work :2;   //bit6~bit7 ��ǰ��ع���״̬ 
     		       
	}Bit;
}STATUS_GROUP4;
//////////////////////  BMS ���͵�״̬��Ϣ//////////////////////////


//////////////////////BMS���͵Ĺ�����Ϣ//////////////////////////
typedef union{
	unsigned char byte;
	struct {
	
	  byte F0_Fire_Warning:2;  //bit0~1 ����Ԥ��
	  byte F2_Ele_Relay_Con:2;  //bit2~3 ����������
    byte flag:4;  //bit4~7    //Ԥ��
	  	 
	}Bit;
}ERROR_GROUP0;

typedef union{
	unsigned char byte;
	struct {
	
	  byte F0_Power_Vol:1;  //bit0��������24V�쳣����
	  byte F1_HLVol_Lock:1;  //bit1 �ߵ�ѹ����
    byte F2_SOC_Change:1;  //bit2 SOC����
    byte F3_BMS_Protect:1;  //bit3 BMS���޹������ұ���
	  byte F4_flag1:1;  //bit4 Ԥ��
    byte F5_Chg_C_Over:1;  //bit5 ����������
    byte F6_flag2:1;  //bit6 Ԥ��
    byte F7_Chg_Ins_Low:1;  //bit7 ����Ե�ͱ���
	  	 
	}Bit;
}ERROR_GROUP6;



typedef union{
	unsigned char byte;
	struct {
	
	  byte St_CHG_Allow:1;  //bit0 �������״̬λ
	  byte St_DisCHG_Allow:1;  //bit1   �ŵ�����״̬λ
    byte F2_DCChg_Neg_Relay_Con:1;  //bit2ֱ����縺�Ӵ���ճ�����ϡ�����������������������
		byte F3_Ele_Relay_Con:1;  //bit3 �ܵ��������Ӵ���ճ��
		byte F4_Ch_Over_I:2;  //bit4~5  ������
	  byte F6_Ins_Err:2;  //bit6~7	��Ե�����
	  	 
	}Bit;
}ERROR_GROUP1;    

typedef union{
	unsigned char byte;
	struct {
	
	  byte F0_Cell_Over_V:2;  //bit0~1 �����ع�ѹ
	  byte F2_Cell_Under_V:2;  //bit2~3 ������Ƿѹ
		byte F4_Temp_Over:2;  //bit4~5     �����¶ȹ���
	  byte flag:2;  //bit6~7	    �������죨ϵͳ��ƥ�䣩Ԥ��
	  	 
	}Bit;
}ERROR_GROUP2; 

typedef union{
	unsigned char byte;
	struct {
	
	  byte F0_Sub_Com_Err:1; //bit0     �ڲ�ͨѶ����
	  byte F1_V_CAN_Err:1;      //bit1  ����CANͨ�Ź���
	  byte F2_P_Con_Err:1;  //bit2    �����̵���ճ��
	  byte F3_PreCha_Failure:1;  //bit3 Ԥ���ʧ�ܡ�����������������������������
		byte F4_N_Con_Err:1;  //bit4     �����̵���ճ��
	  byte F5_DC_Con_Err:1;  //bit5	    ��ӳ�����Ӵ���ճ��
	  byte F6_DisChg_Over_I:2;  //bit6~7	    �ŵ��������
	  	 
	}Bit;
}ERROR_GROUP3; 

typedef union{
	unsigned char byte;
	struct {
	
	  byte F0_SOC_Low:2;  //bit6~7	    SOC��
	  byte F2_SOC_Over:2;  //bit4~5     SOC��
	  byte F4_Bat_Under_V:2;  //bit2~3 ���Ƿѹ
	  byte F6_Bat_Over_V:2;  //bit0~1   ��ع�ѹ

	  	 
	}Bit;
}ERROR_GROUP4; 

typedef union{
	unsigned char byte;
	struct {
	
	 
	  byte F0_FeedB_Over_I:2;  //bit0~1   ��������
		byte F2_Cell_Dif_T:2;  //bit2~3     �����²����
	  byte F4_Cell_Dif_V:2;  //bit4~5	    ��ص���ѹ���
	  byte F6_Cell_Under_T:2;  //bit6~7 �����¶ȵ�
	  	 
	}Bit;
}ERROR_GROUP5;

typedef union{
	unsigned char byte;
	struct {
	
	 
	  byte flag:4;  //bit0~3   Ԥ��
		byte F4_HC_Rel_Error:1;  //bit4     �ȹ������Ӵ������ϣ�ֻ���ˮ��ϵͳ��
	  byte F5_HC_Error:1;  //bit5	    �ȹ������Ӵ������ϣ�ֻ����ȹ���ϵͳ��
	  byte F6_H_Neg_Rel:1;  //bit6 �������ȸ����Ӵ�������
	  byte F7_H_Pos_Rel:1;  //bit7 �������������Ӵ�������
	  	 
	}Bit;
}ERROR_GROUP7;


//////////////////////BMS���͵Ĺ�����Ϣ//////////////////////////

 
//******************************************************************************
//* �����������ⲿ�ı���
//******************************************************************************
extern float g_SOH;

extern VCU_REQUEST VCU_Request;     //0xCFF0BEF
extern VCU_CELLREQUEST VCU_CellRequest;//0x1801D2D0
extern VCU_CONTROL VCU_Control;       //0x1802D2D0
extern VCU_CHGCONTROL VCU_ChgControl;   //0x0C0217A7
extern VCU_PARKINGBRAKE VCU_ParkBrake;  //0x18FEF117



extern STATUS_GROUP1 status_group1;
extern STATUS_GROUP2 status_group2;
extern STATUS_GROUP3 status_group3;
extern STATUS_GROUP4 status_group4;

extern ERROR_GROUP0 Error_Group0;
extern ERROR_GROUP6 Error_Group6;
extern ERROR_GROUP1 Error_Group1;
extern ERROR_GROUP2 Error_Group2;
extern ERROR_GROUP3 Error_Group3;
extern ERROR_GROUP4 Error_Group4;
extern ERROR_GROUP5 Error_Group5;
extern ERROR_GROUP7 Error_Group7;

extern float g_SOH;


extern float BiggestDischargeCurt;              //���ŵ����
extern float BiggestFeedbackCurt;              //����������
extern float BiggestDisCurtContinuous;          //�������ŵ����
extern float BiggestFeedbackCurtContinuous;     //��������������

extern float BiggestDischargePower;                  //���˲ʱ�ŵ繦��
extern float BiggestFeedbackPower;                   //���˲ʱ��������
extern float BiggestDisPowerContinuous;              //�������ŵ繦��
extern float BiggestFeedbackContinuous;              //��������������

extern unsigned int chargeRemainderTime;              //ʣ����ʱ��


extern unsigned char St_heatManage;//��������ȹ���״̬��0û������1���ȹ����У�2cooling


extern unsigned long int chargeAH ;   //ʵ�ʳ���ܰ�ʱ
extern unsigned long int dischargeAH ;//ʵ�ʷŵ��ܰ�ʱ
extern unsigned int Charge_Times;                    //������
extern float DC_ChargePower;//������
extern float VehicleSpeed;                         //�����ź�
extern unsigned char WifeLife;

//******************************************************************************
//* �����������ⲿ�ĺ���
//******************************************************************************

extern void BMS_To_VCU_BasicMsg0(void);
extern void BMS_To_VCU_BasicMsg1(void);
extern void BMS_To_VCU_BasicMsg2(void);
extern void BMS_To_VCU_CHGMsg1(void);

extern void BMS_To_VCU_BatteryMsg1(void);
extern void BMS_To_VCU_BatteryMsg2(void);
extern void BMS_To_VCU_BatteryMsg3(void);
extern void BMS_To_VCU_BatteryMsg4(void);
extern void BMS_To_VCU_BatteryMsg5(void);
extern void BMS_To_VCU_BatteryMsg6(void);
extern void BMS_To_VCU_BatteryMsg7(void);
extern void BMS_To_VCU_BatteryMsg8(void);
extern void BMS_To_VCU_BatCellVolData(void);
extern void BMS_To_VCU_BatCellTempData(void);
extern void BMS_VCU_WIFE(void);
                    


extern void bmsToPcTestCar(void);
extern void bmsToPcTestCar1(void);
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////end/////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////