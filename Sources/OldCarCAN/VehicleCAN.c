//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : S133
//* File Name          : Vehicle.c
//* Author             : Sun Lixue
//* Version            : V1.0.0
//* Start Date         : 2011.6.7
//* Description        : ���ļ�������Ŀͨ��Э�鷢����صı��ĵ�������������
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h"


unsigned char  RelayReq0 =0;

FAULT_GROUP1  Fault_Group1;
FAULT_GROUP2  Fault_Group2;
FAULT_GROUP3  Fault_Group3;
FAULT_GROUP4  Fault_Group4;
//FAULT_GROUP5  Fault_Group5;
FAULT_GROUP6  Fault_Group6;
FAULT_GROUP7  Fault_Group7;
FAULT_GROUP8  Fault_Group8;

unsigned char  BmsCtlStat0 =0; //������λ����أ�������������
unsigned char  SwithState=0;

float BiggestFeedbackPower=0;    //����������
float BiggestDischargePower=0;   //���ŵ繦�� 
float biggestFeedbackCurrent=0;  //��������������
float biggestfeedbackContinuous=0;//������������5min
float biggestDischargeCurrent=0; //�������ŵ����30s
float biggestDisCurtContinuous=0; //�������ŵ����5min
unsigned char pusleLimitSec=0;     //��������������ʱ��


unsigned int DCLeftTime;

unsigned char St_heatManage = 0;//��������ȹ���״̬��0û������1���ȹ����У�2cooling




unsigned char Box_Num_g_highestCellVoltage; //��ߵ����������
unsigned char Box_Posi_g_highestCellVoltage;//��ߵ����������λ��

unsigned char Box_Num_g_lowestCellVoltage;  //��͵����������
unsigned char Box_Posi_g_lowestCellVoltage; //��͵����������λ��

unsigned char Box_Num_g_highestTemperature;  //����¶��������
unsigned char Module_Posi_g_highestTemperature;//����¶�����ģ��

unsigned char State_Box_Online;               //�����������
unsigned char Box_Quantity=6;                   //��������

unsigned char Box_Num_g_lowestTemperature;    //����¶��������
unsigned char Module_Posi_g_lowestTemperature;//����¶�����ģ��

unsigned int Charge_Times;                    //������
unsigned int Year_Battery_Manufacture = 2016;//����������
unsigned int Month_Battery_Manufacture = 6;//��������·�

unsigned long int chargeAH ;   //ʵ�ʳ���ܰ�ʱ
unsigned long int dischargeAH ;//ʵ�ʷŵ��ܰ�ʱ



//Fault_Group5.Bit.Reserve1_Bit = 0;
//Fault_Group7.Bit.Reserve2_Bit = 0;
//Fault_Group7.Bit.Reserve3_Bit = 0;
//Fault_Group7.Bit.Reserve4_Bit = 0;
//Fault_Group7.Bit.Reserve5_Bit = 0;
//Fault_Group7.Bit.Reserve6_Bit = 0;

//*************************************************************************************
//*************************************************************************************
//*****************������BMS����������Ϣ�ӳ���*****************************************
//*************************************************************************************
//******************************************************************************
//* Function name:   BMS_To_VCU_BatMsg1
//* Description:     Message1
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_To_VCU_BatMsg1(void) 
{
    struct can_msg mg;
    char tt=100;
    unsigned int buff;
    unsigned char buff1;
    static unsigned char life=0;
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    life++;
    
    if(Can_g_socValue>=0.995) 
        buff1 = 100*2.5;
    else 
        buff1 = (unsigned char)(Can_g_socValue*100*2.5);
    mg.data[0] = buff1; //SOCֵ    
    
	  buff = (unsigned int)(g_highVoltageV1*50);	//����ܵ�ѹ
	  mg.data[1]= buff;//���ֽ� 
	  mg.data[2]= buff>>8;//���ֽ� 
	  
	  buff = (unsigned int)((g_systemCurrent+3200)*10);	//�ܵ���
	  mg.data[3] = buff;//���ֽ� 
	  mg.data[4] = buff>>8;//���ֽ� 
	  	  
	  buff = (unsigned char) (biggestDisCurtContinuous*0.2);	//�������ŵ����
	  mg.data[5] = buff;//������
	  
	  buff = (unsigned char) (biggestDischargeCurrent*0.2);	//�������ŵ����	  
	  mg.data[6] = buff;//30s�ģ�һ���� 
	 
	  mg.data[7] = life;//
    
    mg.id= 0x18F201F3;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name:   BMS_To_VCU_BatMsg2
//* Description:     Message2
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_To_VCU_BatMsg2(void) 
{
    struct can_msg mg;
    char tt=100;
    unsigned int buff;
    unsigned long DisEnergy=0;
    unsigned long ChaEnergy=0; 
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
   
    buff = (unsigned char)((biggestfeedbackContinuous+1000)*0.2);	//��������������5min
    mg.data[0] = buff;  //
    buff = (unsigned char)((biggestFeedbackCurrent+1000)*0.2);	//��������������30s
    mg.data[1] = buff;  //

    
    DisEnergy = dischargeAH*SYS_NOMINAL_V/1000;
    mg.data[2] = (unsigned char) (DisEnergy&0x00ff);
    mg.data[3] = (unsigned char) ((DisEnergy>>8)&0x00ff);
    mg.data[4] = (unsigned char) ((DisEnergy>>16)&0x00ff);    
   
    ChaEnergy = chargeAH*SYS_NOMINAL_V/1000;
    mg.data[5] = (unsigned char) (ChaEnergy&0x00ff);
    mg.data[6] = (unsigned char) ((ChaEnergy>>8)&0x00ff);
    mg.data[7] = (unsigned char) ((ChaEnergy>>16)&0x00ff);        
    
    mg.id= 0x18F202F3;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name:   BMS_To_VCU_Fault
//* Description:     ���Ϻ�״̬��Ϣ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_To_VCU_Fault(void)  
{
    struct can_msg mg;
    //unsigned int buff;
    char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

    Fault_Group8.Bit.SelfCount= bmsSelfcheckCounter;
	  mg.data[0] = Fault_Group1.byte;
	  mg.data[1] = Fault_Group2.byte;
	  mg.data[2] = Fault_Group3.byte; 
	  mg.data[3] = Fault_Group4.byte;
    mg.data[4] = stateCode;
    mg.data[5] = Fault_Group6.byte;
    mg.data[6] = Fault_Group7.byte;
    mg.data[7] = Fault_Group8.byte;
    
    mg.id= 0x18F205F3;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name:   BMS_To_VCU_CellMsg1
//* Description:     Message1
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_To_VCU_CellExtremumMsg1(void) 
{
    struct can_msg mg;
    char tt=100;
    unsigned int buff;
    
    mg.RTR = FALSE;  
    mg.len = 8;
    mg.prty = 0;
    

	  buff = (unsigned int)(g_highestCellVoltage*1000);
	  mg.data[0] = buff;   //��ص�����ߵ�ѹ ���ֽ�
	  mg.data[1] = buff>>8;//��ص�����ߵ�ѹ ���ֽ�
	  mg.data[2] = Box_Num_g_highestCellVoltage;
	  mg.data[3] = Box_Posi_g_highestCellVoltage;

	  buff = (unsigned int)(g_lowestCellVoltage*1000);
	  mg.data[4] = (unsigned char)buff;//��ص�����͵�ѹ ���ֽ� 
	  mg.data[5] = buff>>8;//��ص�����͵�ѹ ���ֽ�
	  mg.data[6] = Box_Num_g_lowestCellVoltage;
	  mg.data[7] = Box_Posi_g_lowestCellVoltage;	  	  

    mg.id= 0x18F206F3;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}

//******************************************************************************
//* Function name:   BMS_To_VCU_CellMsg2
//* Description:     Message2
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_To_VCU_CellExtremumMsg2(void) 
{
    struct can_msg mg;
    char tt=100;

    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    


	  mg.data[0] = g_highestTemperature;   //��ص�������¶�
	  mg.data[1] = Box_Num_g_highestTemperature;//��ص�������¶��������
	  mg.data[2] = Module_Posi_g_highestTemperature; //��ص�������¶�����ģ����
	 
	  mg.data[3] = g_lowestTemperature;//��ص�������¶�
	  mg.data[4] = Box_Num_g_lowestTemperature; //��ص�������¶��������
	  mg.data[5] = Module_Posi_g_lowestTemperature; //��ص�������¶�����ģ����
	 
	  mg.data[6] = State_Box_Online&0x3f; 
	  mg.data[7] = Box_Quantity;	  	  

    mg.id= 0x18F207F3;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}

//******************************************************************************
//* Function name:   BMS_To_VCU_CellVoltage
//* Description:     �������е����ѹ
//* EntryParameter : number
//* ReturnValue    : None
//******************************************************************************

void BMS_To_VCU_CellVoltage(unsigned char number)  //TO ���أ�CAN0
{
	struct can_msg mg;
	char tt=100;
	unsigned long buff;
	unsigned char i;
	
	//if(number >16)
		//return;
		
  mg.RTR= FALSE;  
  mg.len = 8;
  mg.prty = 0;
//g_cellVol[45][8];

	for(i=0;i<8;i++)
	    mg.data[i]= g_cellVol[number][i];//8N+i�ŵ�ص����ѹ 
	buff = 0x18F208F3+(unsigned long)number*0x100;
			
  mg.id=buff;    
	//MSCAN0SendMsg(mg);
	while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--;

}
//******************************************************************************
//* Function name:   BMS_To_VCU_CellTem
//* Description:     �������е���¶�
//* EntryParameter : number
//* ReturnValue    : None
//******************************************************************************
void BMS_To_VCU_CellTem(unsigned char number)//������ع�����Ϣ����18 ID��0x1C1227EE  
{
	struct can_msg mg;
	unsigned long buff;
	unsigned char i;
	char tt=100;
	
	//if(number >16)
		//return;
	
	mg.RTR= FALSE;  
  mg.len = 8;
  mg.prty = 0;
	
	for(i=0;i<8;i++)
	    mg.data[i]= g_cellTemperature[number][i];//8N+i�ŵ�ص����¶�

	buff = 0x18F235F3+(unsigned long)number*0x100;
			
	mg.id=buff;
	//MSCAN0SendMsg(mg);
  while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--;
}

//******************************************************************************
//* Function name:   BMS_To_VCU_PackMsg
//* Description:     ���pack��Ϣ
//* EntryParameter : number
//* ReturnValue    : None
//******************************************************************************

void BMS_To_VCU_PackMsg(void)  //TO ���أ�CAN0
{
	struct can_msg mg;
	unsigned char tt=100;
	unsigned int buff;
			
  mg.RTR= FALSE;  
  mg.len = 8;
  mg.prty = 0;
  mg.data[0] = 0x03;    //��س���
  buff = (unsigned int)(NOMINAL_CALCULATE*0.2);//�������ϵͳ�����
	mg.data[1] = (unsigned char)buff; 
  
  buff = (unsigned int)(SYS_NOMINAL_V*0.2);//364.8V
	mg.data[2] = (unsigned char)buff;//�������ϵͳ���ѹ 
  
  buff = Charge_Times;//
	mg.data[3] = (unsigned char)buff;//�������ϵͳ���ѹ     
	mg.data[4] = Charge_Times>>8; 
	mg.data[5] = 0x10|Month_Battery_Manufacture; //��ز��Ϻ������·�
  
  mg.data[6] = Year_Battery_Manufacture-2000;   	

	mg.data[7] = BMS_SW_Version[7];
	
  mg.id= 0x18F240F3;
  while((!MSCAN0SendMsg(mg))&&(tt>0))
      tt--; 

}
//******************************************************************************
//* Function name:   BMS_To_VCU_ChargeMsg
//* Description:     ���ͳ��ʱ���ϵͳ������ϵͳ��ѹ�����弫ֵ
//* EntryParameter : number
//* ReturnValue    : None
//******************************************************************************
void BMS_To_VCU_ChargeMsg(void) 
{   
    struct can_msg mg;
    unsigned int buff;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;        
 

    buff = (unsigned int)((g_systemCurrent+3200)*10); //ϵͳ����
    mg.data[0]= buff; //��
    mg.data[1]= buff>>8; //��   
  
    buff = (unsigned int)(g_highVoltageV1*50);        //ϵͳ��ѹ
    mg.data[2]=buff;//
    mg.data[3]=buff>>8;
    
    buff = (unsigned int)(g_highestCellVoltage*1000); //������ߵ�ѹ
    mg.data[4]=buff;    
    mg.data[5]= buff>>8;
    
    buff = (unsigned int)(g_lowestCellVoltage*1000);  //������ߵ�ѹ
    mg.data[6]=buff;    
    mg.data[7]= buff>>8;   

            
    mg.id = 0x18F241F3;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--;
}

void bmsToPcTestCar(void)
{
    struct can_msg mg;
    //unsigned int buff;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    	
	  mg.data[0]= 0xA0; 
  	mg.data[1]= 0;

	  mg.data[2]= 0; 
	  mg.data[3]= 0; 
	  
	  mg.data[4]= 0;
	  mg.data[5]= 0;
	  
	  mg.data[6]= 0;
	  mg.data[7]= 0;

	  mg.id= 0x000c0453; 
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--;
	  
}
//***********************************************************************
//************************************************************************
//*************************the end*************************************
//************************************************************************