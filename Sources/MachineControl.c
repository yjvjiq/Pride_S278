//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : S133
//* File Name          : MachineControl.c
//* Author             : judy
//* Version            : V1.0.0
//* Start Date         : 2016.2.14
//* Description        : ���ļ�����״̬ת���ж�
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h" 
unsigned char HighVolPowerOff = 0;//�г�ģʽ�ϸ�ѹ����
unsigned char OffState=0;//���ģʽ�ϸ�ѹ����
unsigned char PantographOff=0;//�ܵ繭ģʽ�ϸ�ѹ����

unsigned char ChangerINError = 2; //����ChanerIN�ź�ͻȻû��:0:��ChanerIN�ź�;1:��ChangerIN�ź�;2����ȷ��
unsigned char State177End=0;//��ͣ��177״̬,
unsigned char PowerOffError=0;//�г�ģʽ�µ����

unsigned int Error10S=0; //�г����ܵ繭�����µ�10S��ʱ
unsigned int Error5S=0;  //����µ�5S��ʱ
unsigned char E10SOverFlag=0; //10S��ʱ��ɱ�־λ
unsigned int Error20S=0; //�г����ܵ繭�����µ�10S��ʱ���20S��ʱ
unsigned int Error_30s_delay_cnt = 0;
unsigned char TurnOffNRelay=0;//�ϸ��̵�����ɱ�־λ

//******************************************************************************
//* Function name:   stateCodeTransfer
//* Description:     ��ϵͳ�����ڼ�ʱ�̼�⵱ǰ�źţ���û�ź��ˣ���Ҫ�µ�
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void stateCodeTransfer(void) 
{
    static unsigned int Delay30=0;
    static unsigned int CDelay30=0;
    static unsigned int DCDelay30=0;
    static unsigned int DisChangerDelay=0;
    static unsigned int Delay110=0;
    static unsigned int Delay170=0;
    static unsigned int Delay126=0;
    static unsigned int Delay186=0;
    static unsigned int Delay30_81=0;
	static U16 DelayfPowerOffCnt = 0;
    
    //if((OffState == 1)&&(g_BmsModeFlag == DISCHARGING)&&(VehicleSpeed<=8))
    //    PowerOffError = 1;//�г�ģʽ��Ҫ�µ�Ĺ��ϣ����ٽ���8km/h    
        
    if(State177End==1)//��177״̬�½�����State177End��1
    {
        stateCode=179;//����ģʽ
        return;
    }
    else if(TestState==1)
    {
        stateCode=177;//�رռ̵���״̬
        return;
    }
    /////////////////�г�//////////////////////
    if(g_BmsModeFlag == DISCHARGING)
    {
        if(stateCode==11)
        {   
            stateCode=12;
        }
        else if(stateCode==12)
        {
            if((MSDError)||(N_RelayConnetError)||(HighVolPowerOff)||(acc_Connect == 0)
            ||(VCU_Request.Bit.Finish_PreChg)||(plug_DC_Connect == 1))//CC2
                stateCode=46;//MSD��·||����ճ��||��Ҫ�µ�Ĺ���||ON=0||����Ԥ�������
            else if((VCU_Control.Bit.PowerOnOffReq == 1)&&(bmsSelfcheckCounter==1))
                stateCode=14;
        }
        else if(stateCode==14)
        {
            if((acc_Connect == 0)||(PowerOffError == 1)||(VCU_Request.Bit.Finish_PreChg == 1))
                stateCode=44;//||����Ԥ�������
            else if(status_group3.Bit.St_N_Relay == 1)
                stateCode=17;
        }
        else if(stateCode==17)
        {
          
           if((N_RelayDisConError)||(P_RelayConError)||(acc_Connect==0)
           ||(HighVolPowerOff==1)||(VCU_Request.Bit.Finish_PreChg))
                stateCode=44; //�����̵�����·||����ճ��||��Ҫ�µ�Ĺ���||ON=0||����Ԥ�������
           else if(bmsSelfcheckCounter==2)
                stateCode=20;
        }
        else if(stateCode == 20)
        {
            if((P_RelayDisConError==1)||(HighVolPowerOff == 1)||(acc_Connect == 0)||(VCU_Control.Bit.PowerOnOffReq == 2))
            {
				if(VCU_Control.Bit.PowerOnOffReq == 2){ // power off command exist 2s, then turn off the contactor.
					DelayfPowerOffCnt++;
					if(DelayfPowerOffCnt >= 50){ // delay 2s
						stateCode =40;
						DelayfPowerOffCnt = 0;
					}
				}
				else{
					stateCode =40;//������·||��Ҫ�µ�Ĺ���||ON=0
					DelayfPowerOffCnt = 0;
				}
            }
            else if((bmsSelfcheckCounter==3)&&(status_group3.Bit.St_P_Relay)&&(VCU_Request.Bit.Finish_PreChg))
                stateCode=30;//�Լ������=3&&�����̵����պ�&&����Ԥ������
        } 
        else if(stateCode==30)
        {
            if(//(VCU_Control.Bit.PowerOnOffReq == 2)
				((VCU_Control.Bit.PowerOnOffReq != 2)&&(HighVolPowerOff == 1)&&(E10SOverFlag == 1)&&(Error_30s_delay_cnt >= 30000))
				||((VCU_Control.Bit.PowerOnOffReq == 2)&&(E10SOverFlag == 1)&&(Error10S>=10000))//������ʱ10S���յ���ѹ�µ�ָ��                              
//				||((HighVolPowerOff == 1)&&(Error10S>=10000)&&(g_systemCurrent<5))//������ʱ10S�����С��5A
				||((E10SOverFlag)&&(Error20S>=20000) && (g_systemCurrent > 5))//������ʱ10S&&��������5A���ٳ���20S
//				||(plug_DC_Connect == 1)//CC2
				||(Delay30>=400)
            )
            {
                 stateCode=40;
                 Delay30 = 0;
            }
            else if(acc_Connect==0 || VCU_Control.Bit.PowerOnOffReq == 2)//ACC�޻����յ���ѹ�µ�ָ�� 5ms*400=2S
            {
                Delay30++;
            }
            else 
            {
                if(//(VCU_ParkBrake.Bit.Parking_Brake)//û���յ���ѹ�µ������ź�&&�յ��ܵ繭�źų���2S
                (VCU_ChgControl.Bit.downC_Switch)
                ||(VCU_ChgControl.Bit.downC_OK))//פ���ź�,�������أ�������λ����ת��81)
                {
                    Delay30_81++;
                    if(Delay30_81>=400) 
                    {
                        Delay30_81=0; //�˴���0
                        InitialSoc();
                        g_BmsModeFlag = RECHARGING;
                        status_group4.Bit.Mode_BMS_Work = 2;//���״̬
                        status_group3.Bit.St_CHG_Mode = 2;//���ģʽ����AC���
                        stateCode = 81;
                        bmsSelfcheckCounter = 0; 
                    }
                } 
                else 
                {
                    Delay30_81=0; //�˴���0
                }
            } 
              
        } 
        else if(stateCode==40)//�Ͽ������̵���
        {
            if(bmsSelfcheckCounter == 2)
                stateCode = 44; //�Լ������==2&&�����̵����ѶϿ�
        } 
        else if(stateCode==44)
        { 
            if(TurnOffNRelay)
                stateCode=46;
        }
        else if((stateCode==46))
        {
            /////////////////////46 start/////////////////////////
            if((HighVolPowerOff)||(acc_Connect==0)||(RelayErrorPowerOff))
            {
                if(bmsSelfcheckCounter==1)//�޹���
                    stateCode=47;
                /*
                else 
                {
                    DisChangerDelay++;
                    if(DisChangerDelay>=300) //��ʱ�ж�,��ֹbmsSelfcheckCounter�仯�����µ�����
                    {
                        DisChangerDelay=0;//�Լ������û����1,��ճ�����,���ŵ�
                        stateCode=47;
                    }
                }
                */////���ܼӰɣ��ӵĻ����ܾͱ�������ճ��������
            } 
//            else if((plug_DC_Connect==1)&&(bmsSelfcheckCounter==1)) //ֱ�����ǹ,��ճ������
//            {
//                //InitialSoc();//SOC���㷽ʽ�仯 ????
//                //First_g_socValue=g_socValue;
//                //StoreAHSOC=g_socValue;
//                //Can_g_socValue_Start=Can_g_socValue;
//                
//                g_BmsModeFlag = FASTRECHARGING;
//                status_group4.Bit.Mode_BMS_Work = 2;//���״̬
//                status_group3.Bit.St_CHG_Mode = 1;//���ģʽ����DC���
//                stateCode = 141;
//                bmsSelfcheckCounter = 0;
//            }
            
            else if((VCU_ChgControl.Bit.downC_Switch)&&(VCU_ChgControl.Bit.downC_OK)&&(bmsSelfcheckCounter==1))
            {
                //��������&&������λ&&�Լ������==1
                InitialSoc();
                g_BmsModeFlag = RECHARGING;
                status_group4.Bit.Mode_BMS_Work = 2;//���״̬
                status_group3.Bit.St_CHG_Mode=2;//���ģʽ����AC���
                stateCode = 81;
                bmsSelfcheckCounter = 0;            
            }
            else if(VCU_Control.Bit.PowerOnOffReq == 1)//�ϵ�
            {
                stateCode=11;
                status_group4.Bit.Mode_BMS_Work = 1;//BMS��ǰ����״̬=�ŵ�״̬ 
            }
        }
        /////////////////////46 end///////////////////////// 
        
    }
    ///////////////�ܵ繭��� //////////////////
    else if(g_BmsModeFlag == RECHARGING)
    {
        if(stateCode==81)
        {   
            if((VCU_ChgControl.Bit.downC_Switch)&&(VCU_ChgControl.Bit.downC_OK)&&(VCU_ParkBrake.Bit.Parking_Brake))
                stateCode=82;
        }
        else if(stateCode==82)
        {  
            if((MSDError==1)||(CHG_N_RelayConError==1)||(PantographOff)
            ||(VCU_ChgControl.Bit.downC_Switch == 0)||(VCU_ChgControl.Bit.downC_OK == 0)||(VCU_ParkBrake.Bit.Parking_Brake==0))
                stateCode=126; //MSD��·||��縺ճ��||��Ҫ�µ����||||��������==0||������λ==0||פ���ź�==0
            else if(bmsSelfcheckCounter==1)
                stateCode=84; //�Լ������==1&&������λ&&��������==1&&פ���ź�==1
        }
        else if(stateCode==84)
        {
            if((VCU_ChgControl.Bit.downC_Switch == 0)||(VCU_ChgControl.Bit.downC_OK == 0)||(VCU_ParkBrake.Bit.Parking_Brake==0)||(PantographOff))
                stateCode=124;//��������==0||������λ==0||פ���ź�==0||��Ҫ�µ����
            else if((BmsCtlStat0&0x08)!=0)//������λ&&��������
                stateCode=87;//��縺�ѱպ�
        }
        else if(stateCode==87)
        {
            if((CHG_N_RelayDisConError)||(CCHG_RelayConError)||(PantographOff)
             ||(VCU_ChgControl.Bit.downC_Switch == 0)||(VCU_ChgControl.Bit.downC_OK == 0)||(VCU_ParkBrake.Bit.Parking_Brake==0)) 
                stateCode=124;//��ѹ���2�й���||���µ�Ĺ���||��������==0||������λ==0������WIFI)||פ���ź�
            else if(bmsSelfcheckCounter==2) 
                stateCode=90;//�Լ������==2&&��������==1&&������λ==1������WIFI)
        }
        else if(stateCode == 90)
        {
            if((VCU_ChgControl.Bit.downC_Switch == 0)||(VCU_ChgControl.Bit.downC_OK == 0)
            ||(PantographOff)||(CCHG_RelayDisConError)||(VCU_ParkBrake.Bit.Parking_Brake==0))
                stateCode=120; //��ѹ���3�й���||���µ�Ĺ���||��������==0||������λ==0������WIFI)||פ���ź� 
            else if(bmsSelfcheckCounter==3) 
                stateCode=110;//�Լ������==3&&��������==1&&������λ==1&&��翪��==1
        }
        else if(stateCode==110)
        {
            if((slowRechargeFinished == 1)||((PantographOff)&&(Error10S>=10000)&&(g_systemCurrent>-5))//��ʱ10S�µ�
			||((PantographOff == 1)&&(E10SOverFlag)&&(Error20S>=20000)&&(g_systemCurrent <= -5))//������ʱ10S&&��������5A���ٳ���20S
			//||(plug_DC_Connect == 1)
            ){
                stateCode=120;//�������ɻ������µ�Ĺ��ϻ��߼�⽵������==0||������λ==0||��翪��==0    
            }
            else if((VCU_ChgControl.Bit.downC_Switch == 0)
            ||(VCU_ChgControl.Bit.downC_OK == 0)
            ||(VCU_ParkBrake.Bit.Parking_Brake==0)
			||((fastendflag == 1) && (PantographOff != 1)) // charge finished and no fault.
            )//����ź���     5ms * 400 = 2.0S
            {
                CDelay30++;
				if(CDelay30 >= 400){
					stateCode = 120;
					CDelay30 = 0;
				}
            }  
        } 
        else if(stateCode==120)//�Ͽ��ܵ繭�̵���
        {
            if(bmsSelfcheckCounter == 2)//�Լ������=2
                stateCode = 124;
        } 
        else if(stateCode==124)
        { 
            if(TurnOffNRelay)//ȷ����縺�̵����ѶϿ�
                stateCode=126;
        } 
        else if((stateCode==126))
        {
            if(((bmsSelfcheckCounter==1)&&(g_highVoltageV3<200))||(acc_Connect==0)||(OffState)||(PantographOff))//�޹���
                stateCode=127;
            else if((bmsSelfcheckCounter==1)&&(g_highVoltageV3>200)&&(acc_Connect==1)) 
            {
                stateCode=30;//���V3����200V��֤���Ǵ�30�������ģ����ö�����������
                g_BmsModeFlag =DISCHARGING;
                status_group4.Bit.Mode_BMS_Work = 1;//BMS��ǰ����״̬=�ŵ�״̬ 

            }
            /*if((plug_DC_Connect==1)&&(bmsSelfcheckCounter==1)) //ֱ�����ǹ,��ճ������
            {
                InitialSoc();//SOC���㷽ʽ�仯
                First_g_socValue=g_socValue;
                StoreAHSOC=g_socValue;
                Can_g_socValue_Start=Can_g_socValue;
                
                g_BmsModeFlag = FASTRECHARGING;
                stateCode = 141;
                bmsSelfcheckCounter = 0;
            }
            else if((VCU_ChgControl.Bit.downC_OK == 1)&&(VCU_ParkBrake.Bit.Parking_Brake == 1)//פ���źţ�������λ
              &&(VCU_ChgControl.Bit.downC_Switch == 1)&&(bmsSelfcheckCounter==1)&&(OffState == 0)) //��������
                stateCode=81;
            else if(acc_Connect==0)
            {
                if(bmsSelfcheckCounter==1)//�޹���
                    stateCode=127;
                else 
                {
                    DisChangerDelay++;
                    if(DisChangerDelay>=300) //��ʱ�ж�,��ֹbmsSelfcheckCounter�仯�����µ�����
                    {
                        DisChangerDelay=0;//�Լ������û����1,��ճ�����,���ŵ�
                        stateCode=127;
                    }
                }
            }
            else if((acc_Connect == 1)&&(plug_DC_Connect == 0)&&(VCU_ChgControl.Bit.downC_OK == 0)&&(VCU_ParkBrake.Bit.Parking_Brake == 0)&&(VCU_ChgControl.Bit.downC_Switch == 0)) 
            {
                stateCode = 11;
                status_group4.Bit.Mode_BMS_Work = 1;//�ŵ�״̬ 
                bmsSelfcheckCounter = 0;
            } */
        } 
    }
    ///////////////////////////���/////////////////////////////////
    else if(g_BmsModeFlag==FASTRECHARGING)
    {
        if(stateCode==141)
        {   
            InsRelayControl=1;//���ʱ�ɾ�Ե���Ƹ�SBMS
            if((status_group3.Bit.St_N_Relay == 0)&&(status_group3.Bit.St_P_Relay == 0)
            &&(CC2VOL<=CC2VOLHIGH)&&(CC2VOL>=CC2VOLLOW)) //CC2��ѹ�ڷ�Χ��
                stateCode=142;
        }
        else if(stateCode==142)
        {  
            if((MSDError)||(CHG_N_RelayConError)||(plug_DC_Connect == 0)||(OffState))
                stateCode=186;//��ѹ���1�й���||���µ�Ĺ���||CC2==0 ||ChargeIN==0
            else if((plug_DC_Connect == 1)&&(bmsSelfcheckCounter==1))
                stateCode=144;
        }
        else if(stateCode==144)
        {
            if((plug_DC_Connect == 0)||(OffState))
                stateCode=184; //���µ�Ĺ���||CC2==0 ||ChargeIN==0
            else if((plug_DC_Connect == 1)&&((BmsCtlStat0&0x08)!=0))
                stateCode=147;//�����̵����Ѿ��պ�&&CC2==1&&ChargeIN==1
        }
        else if(stateCode==147)
        {
           if((CHG_N_RelayDisConError==1)||(DCCHG_RelayConError==1)||(plug_DC_Connect == 0)||(OffState))
                stateCode=184;//��ѹ���2�й���||���µ�Ĺ���||CC2==0 ||ChargeIN==0
           else if((plug_DC_Connect == 1)&&(bmsSelfcheckCounter==2))
                stateCode=150;//�Լ������==2&&CC2==1&&ChargeIN==1
        }
        else if (stateCode == 150)
        {                                      
            if((plug_DC_Connect == 0)||(DCCHG_RelayDisConError==1)||(OffState))
                stateCode =180;//��ѹ���3�й���||���µ�Ĺ���||CC2==0 ||ChargeIN==0
            else if((bmsSelfcheckCounter==3)&&(plug_DC_Connect == 1)&&(status_group2.Bit.St_CHG_Connect == 2))
                stateCode=170;
        }
        else if(stateCode==170)
        {
            if(((OffState)&&(Error5S>=5000))||(DCDelay30>=400))//
            { 
                 stateCode=180;
                 DCDelay30 = 0; 
            }  
            else if(plug_DC_Connect==0)//CC2��    5ms*300=2.5S
            {                
                DCDelay30++; 
            }
        } 
        else if(stateCode==180)//�Ͽ����̵���
        {
            if(bmsSelfcheckCounter == 2)
                stateCode = 184;
        } 
        else if(stateCode==184)
        { 
            if(TurnOffNRelay)//ȷ�����Ͽ�
                stateCode=186;
        } 
        else if((stateCode==186))
        {
			if(bmsSelfcheckCounter == 1){
                stateCode=187;
			}
        } 
    } 
}
//*************************************************************************
//**************************the end *********************************************
//************************************************************************************
//******************************************************************************************
