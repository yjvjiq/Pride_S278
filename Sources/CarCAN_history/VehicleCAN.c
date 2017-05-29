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

CAN450BYTE7      Can450Byte7;
CAN450BYTE0      Can450Byte0;

CAN554BYTE0      Can554Byte0;
CAN554BYTE1      Can554Byte1;
CAN554BYTE2      Can554Byte2;
CAN554BYTE3      Can554Byte3;
CAN554BYTE4      Can554Byte4;
CAN554BYTE5      Can554Byte5;
CAN554BYTE6      Can554Byte6;
CAN554BYTE7      Can554Byte7;


CAN1818BYTE6     Can1818Byte6;
CAN1818BYTE7     Can1818Byte7;
CAN1818BYTE8     Can1818Byte8;

CAN181CBYTE3     Can181cByte3;
CAN181CBYTE5     Can181cByte5;

CANF100BYTE3     Canf100Byte3;


unsigned char  BmsCtlStat0 =0; //������λ����أ�������������
unsigned char  SwithState=0;

float BiggestFeedbackPower=0;    //����������
float BiggestDischargePower=0;   //���ŵ繦�� 
float biggestFeedbackCurrent=0;  //��������������
float biggestDischargeCurrent=0; //�������ŵ����
unsigned char pusleLimitSec=0;     //��������������ʱ��


unsigned int DCLeftTime;
unsigned char st_heating;//������ؼ���״̬��0δ���ȣ�1Ԥ�����У�2�߳��߼�����
unsigned char St_heatManage = 0;//��������ȹ���״̬��0û������1���ȹ����У�2cooling
unsigned char BattHeatSave;     //������ر���״̬��0δ���£�1����
unsigned char BattPTCReq;        //������ؼ���PTC��������0δ����1������

unsigned long int chargeAH ;   //ʵ�ʳ���ܰ�ʱ
unsigned long int dischargeAH ;//ʵ�ʷŵ��ܰ�ʱ

unsigned char LCellBoxNum=0;//��͵�ѹ��Ӧ�����
unsigned char LCellLocation=0;//��͵����ѹ����λ��
unsigned char HCellBoxNum=0;//��ߵ�ѹ��Ӧ�����
unsigned char HCellLocation=0;//��ߵ����ѹ����λ��

unsigned char LTemBoxNum=0;//����¶ȶ�Ӧ�����
unsigned char LTemLocation=0;//����¶�����λ��
unsigned char HTemBoxNum=0;//����¶ȶ�Ӧ�����
unsigned char HTemLocation=0;//����¶�����λ��


unsigned char Box1HTem=0;
unsigned char Box2HTem=0;
unsigned char Box3HTem=0;
unsigned char Box4HTem=0;
unsigned char Box5HTem=0;
unsigned char Box6HTem=0;
unsigned char Box1LTem=0;
unsigned char Box2LTem=0;
unsigned char Box3LTem=0;
unsigned char Box4LTem=0;
unsigned char Box5LTem=0;
unsigned char Box6LTem=0;

unsigned char ChaStartH=0;
unsigned char ChaStartM=0;
unsigned char ChaStartS=0;
unsigned char ChaOverH=0;
unsigned char ChaOverM=0;
unsigned char ChaOverS=0;
float ChangerVol=0;
float ChangerCurt=0;

//*************************************************************************************
//*************************************************************************************
//*****************������BMS����������Ϣ�ӳ���*****************************************
//*************************************************************************************
//******************************************************************************
//* Function name:   BMS1_transmit1818
//* Description:     ��������,100ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS1_transmit1818(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char tt=100;
    
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

	  buff = g_highVoltageV1*10;	//����ܵ�ѹ
	  mg.data[0]= buff;//���ֽ� 
	  mg.data[1]= buff>>8;//���ֽ� 
	  
	  buff = 10*g_systemCurrent-3200;	//�ܵ���
	  mg.data[2]= buff;//���ֽ� 
	  mg.data[3]= buff>>8;//���ֽ� 

    if(g_socValue>=0.995) 
        buff=100;
    else 
        buff = (unsigned int)(g_socValue*100*2.5);
    mg.data[4] = buff; //SOCֵ
    
    mg.data[5] = Can1818Byte6.byte; //����
    mg.data[6] = Can1818Byte7.byte; //����
    mg.data[7] = Can1818Byte8.byte; //����
    
    mg.id= 0x1818D0F3;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}

//******************************************************************************
//* Function name:   BMS2_transmit1819
//* Description:     ��������,100ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS2_transmit1819(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char life;
    unsigned char tt=100;
    
    life++;
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

	  buff = (unsigned int)(g_lowestCellVoltage*100);
	  mg.data[0]= (unsigned char)buff;//��ص�����͵�ѹ ���ֽ� 
	  mg.data[1]= ((buff>>8)|(LCellBoxNum<<4));//��ص�����͵�ѹ ���ֽ�
	  	  
	  //���ֽ�4bitΪ���

	  buff = (unsigned int)(g_highestCellVoltage*1000);
	  mg.data[2]= buff;//��ص�����ߵ�ѹ ���ֽ�
	  mg.data[3]= ((buff>>8)|(HCellBoxNum<<4));//��ص�����ߵ�ѹ���ֽ�
	  
	  //���ֽ�4bitΪ���
    
    mg.data[4] = g_lowestTemperature+40-40;  //����¶�
    mg.data[5] = g_highestTemperature+40-40; //����¶�
    mg.data[6] = 0;  //
    mg.data[7] =life; //
    
    mg.id= 0x1819D0F3;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}


//******************************************************************************
//* Function name:   BMS3_transmit181A
//* Description:     ��������,100ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS3_transmit181A(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char life;
    unsigned char tt=100;
    
    life++;
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

	  buff = biggestDischargeCurrent*10+3200;	//�������ŵ����
	  mg.data[0]= buff;//���ֽ� 
	  mg.data[1]=buff>>8;//���ֽ� 
	  
	  buff = biggestFeedbackCurrent*10+3200;	//������������
	  mg.data[2]= buff;// ���ֽ� 
	  mg.data[3]= buff>>8;//���ֽ�
	  
	  buff = C*10+3200;	//����������������
	  mg.data[4]= buff;//���ֽ� 
	  mg.data[5]=buff>>8;//���ֽ� 
    
    buff = HIGHEST_ALLOWED_CHARGE_V*10;//����������ܵ�ѹ
    mg.data[6] = buff;  //
    mg.data[7] = buff>>8; //
    
    mg.id= 0x181AD0F3;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}

//******************************************************************************
//* Function name:   BMS4_transmit181B
//* Description:     ��������,100ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS4_transmit181B(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

    mg.data[0] = 0;  //����
    buff = HIGHEST_ALLOWED_CHARGE_V*10;//�������ŵ��ܵ�ѹ
    mg.data[1] = buff;  //
    mg.data[2] = buff>>8; // 
	  
	  buff = (-biggestFeedbackCurrent)*10+3200;	//��������������
	  mg.data[3]= buff;// ���ֽ� 
	  mg.data[4]= buff>>8;//���ֽ�
	  
    
    buff = HIGHEST_ALLOWED_CHARGE_CV*100;
    mg.data[5] = buff;  //
    mg.data[6] = buff>>8; //
    
    mg.data[7] = HIGHEST_ALLOWED_Tem+40;
    
    mg.id= 0x181BD0F3;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}

//******************************************************************************
//* Function name:   BMS5_transmit181C
//* Description:     ��������,100ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS5_transmit181C(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

    
    buff = LOWEST_ALLOWED_DISCHARGE_V*10;//�������ŵ��ܵ�ѹ
    mg.data[0] = buff;  //
    mg.data[1] = buff>>8; // 
	  
	  mg.data[2]= Can181cByte3.byte;//����������	  
	  mg.data[3] = 0;    
    mg.data[4] = Can181cByte5.byte;  //���ǹ����״̬�����ϴ���ʽ
    mg.data[5] = 0;     
    mg.data[6] =0;
    mg.data[7] =0;
    
    mg.id= 0x181CD0F3;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}


//******************************************************************************
//* Function name:   BMS6_transmit181D
//* Description:     ��������,100ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS6_transmit181D(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

    
    mg.data[0] = 0;  
    mg.data[1] = 0;  
	  
	  mg.data[2] = 0;	  
	  mg.data[3] = 0;    
    mg.data[4] = 0;
    mg.data[5] = 0;     
    mg.data[6] = 0;
    mg.data[7] = 0;
    
    mg.id= 0x181DD0F3;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name:   BMS7_transmit18F2
//* Description:     ��������(������Ϣ)100ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS7_transmit18F2(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

    
    mg.data[0] = 0;  
    mg.data[1] = 0;  
	  
	  mg.data[2] = 0; 
	  mg.data[3] = 0;    
    mg.data[4] = 0;
    mg.data[5] = 0;     
    mg.data[6] = 0;
    mg.data[7] = 0;
    
    mg.id= 0x18F214F3;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}

//******************************************************************************
//* Function name:   BMS_transmitFF2A
//* Description:     ��������(�������ر���),1000ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_transmitFF2A(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

       
    mg.data[0] = HCellBoxNum; //������ߵ�ѹ���
    mg.data[1] = HCellLocation; //������ߵ�ѹ����λ�� 
	  
	  mg.data[2] = LCellBoxNum;//������͵�ѹ���	  
	  mg.data[3] = LCellLocation;//������͵�ѹ����λ��     
    mg.data[4] = HTemBoxNum;//�¶�������
    mg.data[5] = HTemLocation;//�¶��������λ��     
    mg.data[6] = LTemBoxNum;//�¶�������
    mg.data[7] = LTemLocation;//�¶��������λ�� 
    
    mg.id= 0x18FF2AF4;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name:   BMS_transmitFF2C
//* Description:     ��������(�������ر���),1000ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_transmitFF2C(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

    buff=2.8*100;   
    mg.data[0] = buff; //����ͷ�ֵ(1��)
    mg.data[1] = buff>>8; //
	  
	  buff=3.65*100;
	  mg.data[2] = buff;//����߷�ֵ(1��)	  
	  mg.data[3] = buff>>8;//  
	    
    mg.data[4] = -20+40;//�¶ȵͷ�ֵ(1��)
    
    mg.data[5] = 45+40;//�¶ȸ߷�ֵ(1��)
      
    mg.data[6] = 0;//����
    mg.data[7] = 0;//���� 
    
    mg.id= 0x18FF2CF4;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}

//******************************************************************************
//* Function name:   BMS_transmitFF2D
//* Description:     ��������(�������ر���),1000ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_transmitFF2D(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char t;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

       
    mg.data[0] = BOX_NUMBER; //��������
    mg.data[1] = ALL_CELL_NUMBER; //��ش���
	  
	  mg.data[2] = ALL_Tem_NUMBER;//�¶��ܸ���	  
	  mg.data[3] = 0;//
	  
	  buff = SYS_NOMINAL_V*10;//��ܵ�ѹ     
    mg.data[4] = buff;
    mg.data[5] = buff>>8;
          
    mg.data[6] = SYS_KWH*2/3;//�����
    
    mg.data[7] = g_socValue*C*g_highVoltageV1*3/2000;//ʣ������soc*240*V1/1000*�ֱ���
        
    mg.id= 0x18FF2DF4;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name:   BMS_transmitFF2E
//* Description:     ��������(�������ر���),1000ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_transmitFF2E(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char t;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

       
    mg.data[0] = BOX1_CELL_NUMBER; // ����1�������
    mg.data[1] = BOX2_CELL_NUMBER; // ����2�������
	  
	  mg.data[2] = BOX3_CELL_NUMBER; // ����3�������	  
	  mg.data[3] = BOX4_CELL_NUMBER; // ����4�������;//
	      
    mg.data[4] = BOX5_CELL_NUMBER; // ����5�������
    mg.data[5] = BOX6_CELL_NUMBER; // ����6�������
          
    mg.data[6] = 0;//����
    mg.data[7] = 0;//����
        
    mg.id= 0x18FF2EF4;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name:   BMS_transmitFF30
//* Description:     ��������(�������ر���),1000ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_transmitFF30(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char t;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

       
    mg.data[0] = BOX1_TEM_NUMBER; // ����1�¶ȸ���
    mg.data[1] = BOX2_TEM_NUMBER; // ����2�¶ȸ���
	  
	  mg.data[2] = BOX3_TEM_NUMBER; // ����3�¶ȸ���  
	  mg.data[3] = BOX4_TEM_NUMBER; // ����4�¶ȸ���
	      
    mg.data[4] = BOX5_TEM_NUMBER; // ����5�¶ȸ���
    mg.data[5] = BOX6_TEM_NUMBER; // ����6�¶ȸ���
          
    mg.data[6] = 0;//����
    mg.data[7] = 0;//����
        
    mg.id= 0x18FF30F4;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name:   BMS_transmitF100
//* Description:     ��������(�������ر���),1000ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_transmitF100(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char t;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

    t=(0x24|PARA_NUM);    
    mg.data[0] = t; // ����1�¶ȸ���
    mg.data[1] = 0; // ����
	  
	  mg.data[2] = Canf100Byte3.byte;
	  mg.data[3] = 0;
	      
    mg.data[4] = 0;
    mg.data[5] = 0;
          
    mg.data[6] = 0;//ʵ������(AH)
    mg.data[7] = 0;//�����(AH)
                                                    
    mg.id= 0x18F100F4;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name:   BMS_transmitFF33
//* Description:     ��������(�������ر���),1000ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_transmitFF33(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char t;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0] = Box1HTem; // ����1����¶�
    mg.data[1] = Box1LTem; // ����1����¶�
	  
	  mg.data[2] = Box2HTem; // ����2����¶�
	  mg.data[3] = Box2LTem; // ����2����¶�
	      
    mg.data[4] = Box3HTem; // ����3����¶�
    mg.data[5] = Box3LTem; // ����3����¶�
          
    mg.data[6] = Box4HTem; // ����4����¶�
    mg.data[7] = Box4LTem; // ����4����¶�
        
    mg.id= 0x18FF33F4;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name:   BMS_transmitFF34
//* Description:     ��������(�������ر���),1000ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_transmitFF34(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char t;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0] = Box5HTem; // ����5����¶�
    mg.data[1] = Box5LTem; // ����5����¶�
	  
	  mg.data[2] = Box6HTem; // ����6����¶�
	  mg.data[3] = Box6LTem; // ����6����¶�
	      
    mg.data[4] = 0;
    mg.data[5] = 0;
          
    mg.data[6] = 0;
    mg.data[7] = 0;
        
    mg.id= 0x18FF34F4;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//******************************************************************************
//* Function name:   BMS_transmitFF50
//* Description:     ��������(�������ر���),500ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_transmitFF50(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char t;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0] = ChaStartH; // ��翪ʼʱ��(ʱ)BCD��
    mg.data[1] = ChaStartM; // ��翪ʼʱ��(��)BCD��	  
	  mg.data[2] = ChaStartS; // ��翪ʼʱ��(��)BCD��
	  
	  mg.data[3] = ChaOverH; // ������ʱ��(ʱ)BCD��	      
    mg.data[4] = ChaOverM; // ������ʱ��(��)BCD��
    mg.data[5] = ChaOverS; // ������ʱ��(��)BCD��
    
    //����ѹ
    buff=ChangerVol*10;//�����������ѹ      
    mg.data[6] = buff;
    mg.data[7] = buff>>8;
        
    mg.id= 0x18FF50F4;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}

//******************************************************************************
//* Function name:   BMS_transmitFF51
//* Description:     ��������(�������ر���),500ms
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMS_transmitFF51(void) 
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char t;
    unsigned char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0] = ChaStartH; // ��翪ʼʱ��(ʱ)BCD��
    mg.data[1] = ChaStartM; // ��翪ʼʱ��(��)BCD��	  
	  mg.data[2] = ChaStartS; // ��翪ʼʱ��(��)BCD��
	  
	  mg.data[3] = ChaOverH; // ������ʱ��(ʱ)BCD��	      
    mg.data[4] = ChaOverM; // ������ʱ��(��)BCD��
    mg.data[5] = ChaOverS; // ������ʱ��(��)BCD��
    
    //����ѹ
    buff=ChangerCurt*10;//�����������ѹ      
    mg.data[6] = buff;
    mg.data[7] = buff>>8;
        
    mg.id= 0x18FF51F4;
    while((!MSCAN0SendMsg(mg))&&(tt>0))
        tt--; 
}
//***********************************************************************
//************************************************************************
//*************************the end*************************************
//************************************************************************