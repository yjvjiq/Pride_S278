//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : ProcessBMU.c
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2014.5.7
//* Description        : ���ļ�ר�����ڴ�����յ���BMU���ݣ��Ӷ��ó����弫ֵ�͵���ֵ
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "derivative.h" /* include peripheral declarations */
#include "BMS20.h" 


#pragma DATA_SEG __GPAGE_SEG PAGED_RAM  
U8 g_group;  //BMU1 ��ţ� ��0~18
U16 g_singleCellVoltage[BMU_NUMBER][SIX802_NUMBER][CELL_NUMBER];  //BMU1��, 6802���飩��, �����غ�
U8 g_singleCellTemperature[BMU_NUMBER][SIX802_NUMBER][Tem_NUMBER];//BMU1��, 6802���飩��, �����غ�
#pragma DATA_SEG DEFAULT 


float g_highestCellVoltage=0; //������ߵ�ѹ
float g_lowestCellVoltage=0;  //������͵�ѹ
U8 g_highestCellVoltage_Num=0; //������ߵ�ѹ���
U8 g_lowestCellVoltage_Num=0;  //������͵�ѹ���

float g_averageVoltage;       //ƽ�������ѹ
float g_systemVoltage;      //ϵͳ��ѹ=�����ѹ�ۼ��ܵ�ѹ

U8 g_highestTemperature=0; //��������¶�
U8 g_lowestTemperature=0;  //��������¶�
U8 g_highestTemperature_Num=0; //��������¶ȱ��
U8 g_lowestTemperature_Num=0;  //��������¶ȱ��

U8 g_averageTemperature; //����ƽ���¶�
U8 Tavg; //��оƽ���¶�

U8 LowVolNum=0;
U8 HighVolNum=0;
U8 HighTemNum=0;
U8 LowTemNum=0;

U8 highestCellVolNum=0;
U8 lowestCellVolNum=0;
U8 highestCellTempNum=0;
U8 lowestCellTempNum=0;

U16 g_singleCellVmax[BMU_NUMBER];
U16 g_singleCellVmin[BMU_NUMBER];
U8 g_singleCellVmax_Num[BMU_NUMBER];
U8 g_singleCellVmin_Num[BMU_NUMBER];           
U8 g_singleCellTmax[BMU_NUMBER];
U8 g_singleCellTmin[BMU_NUMBER];
U8 g_singleCellTmax_Num[BMU_NUMBER];
U8 g_singleCellTmin_Num[BMU_NUMBER]; 
 
U8 g_bmu2_number_v[BMU_NUMBER];
U8 g_cell_number_v[BMU_NUMBER][5];
U8 g_bmu2_number_t[BMU_NUMBER];
U8 g_cell_number_t[BMU_NUMBER][5];

//unsigned long recogBMUtoBMSmessage;//BMS BMUͨ�ű�ʶ��Ϣ
U8 recogBMStoBMUflag = 0;//BMS��BMU��ʶ�ɹ���־
//U8 g_cellVol[CELL_VOL_GROUP][6];//
//U8 g_cellTemperature[CELL_TEMP_GROUP][6];
U8 g_cellVol[BMU_NUMBER][36]; // re-group all the cell voltage. 18 BMU and 36 cells in each BMU.
U8 g_cellTemperature[BMU_NUMBER][3]; // re-group all the cell temperature. 18 BMU and 3 temperatures in each BMU.
	
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
U32 g_circleFlag = 0; //һ��ѭ����ɵı�־
U32 g_configFlag = 0;//�յ�������Ϣ��־�������ж�BMU������6805�����͵������ 

//******************************************************************************
//* Function name:   BMU_initial
//* Description:     �ϵ����BMU�������ݣ�ȷ�������¶Ⱥ͵�ѹ��û����
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMU_initial(void) 
{
    U16 t=0;
    
    //***********BMU�Լ�*******************************************       
    while((!bmuProcess2())&&(t<65000))    	        
    {        
        t++;      
        if(t>=60000) //��ʱ���ϱ���
        {
            //t= 60000;
            //Can08f0Byte5.Bit.F3_innerComm=1;  //t0 vcu
            g_caution_Flag_3 |=0x01;    //to pc
                     
        }/////end of BMUͨ���ж�  
        else 
        {            
            //Can08f0Byte5.Bit.F3_innerComm=0; //to vcu 
            g_caution_Flag_3 &=0xfe;  //���ϱ��ڲ�ͨѶ����   to pc 
        }  
        _FEED_COP();   //2s�ڲ�ι�ڹ�����ϵͳ��λ 
    }

}
//******************************************************************************
//* Function name:   BMU_Processure
//* Description:     �Խ��յ���BMU�����ѹ���¶Ƚ��н���
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMU_Processure(void)
{
    U32 framID;
    U16 i,boxNumber=0;
	   
    if((Int_Flag&0x08)==0x08) //�����յ�1�����ݣ��������ݴ���
    {
        Int_Flag &= 0xf7;//���жϱ�־		
          			  		
        g_group = 0;  		


        framID = g_mboxID;      
        g_group = framID&0x000000ff;
        g_group--;
        /*
        framID = framID>>4;
        framID = framID & 0x03ffffff;
        */
        framID = framID>>8;
        framID = framID & 0x003fffff;

		switch(framID)  //��������
		{
            case 0x018FF11://BMU��ʶ��Ϣ��Ӳ���汾������汾��ͨѶЭ��汾�ȣ�0x18FF110x
                //if(recogBMUtoBMSmessage != bufL)
                //	recogBMStoBMUflag = 0; //��ʶʧ��
                //else
                recogBMStoBMUflag = 1; //��ʶ�ɹ�								
                break;	  
            case 0x018FF13://���������Ϣ1��6802�ĸ�������ز�ѹ����������������CC2
                switch(g_group) 
                {
                    case 0:
                      g_configFlag |= 0x00000001;
                      break;
                    case 1:
                      g_configFlag |= 0x00000002;
                      break;
                    case 2:
                      g_configFlag |= 0x00000004;
                      break;
                    case 3:
                      g_configFlag |= 0x00000008;
                      break;
                    case 4:
                      g_configFlag |= 0x00000010;
                      break;
                    case 5:
                      g_configFlag |= 0x00000020;
                      break;
                    case 6:
                      g_configFlag |= 0x00000040;
                      break;
                    case 7:
                      g_configFlag |= 0x00000080;
                      break;
                    case 8:
                      g_configFlag |= 0x00000100;
                      break;
                    case 9:
                      g_configFlag |= 0x00000200;
                      break;
                    case 10:
                      g_configFlag |= 0x00000400;
                      break;
                    case 11:
                      g_configFlag |= 0x00000800;
                      break;
                    case 12:
                      g_configFlag |= 0x00001000;
                      break;
                    case 13:
                      g_configFlag |= 0x00002000;
                      break;
                    case 14:
                      g_configFlag |= 0x00004000;
                      break;
                    case 15:
                      g_configFlag |= 0x00008000;
                      break;
                    case 16:
                      g_configFlag |= 0x00010000;
                      break;
                    case 17:
                      g_configFlag |= 0x00020000;
                      break;      				                				          
                }
			      
			    g_bmu2_number_v[g_group] = g_mboxData[boxNumber][0]&0x07; //&0x07;//BMU1��6802�ĸ���
				g_bmu2_number_t[g_group] = (g_mboxData[boxNumber][0]&0x70)>>4; //&0x07;
        
				g_cell_number_v[g_group][0]=g_mboxData[boxNumber][1]&0x0f;
				g_cell_number_t[g_group][0]=(g_mboxData[boxNumber][1]&0xf0)>>4;

				g_cell_number_v[g_group][1]=g_mboxData[boxNumber][2]&0x0f;
				g_cell_number_t[g_group][1]=(g_mboxData[boxNumber][2]&0xf0)>>4;

				g_cell_number_v[g_group][2]=g_mboxData[boxNumber][3]&0x0f;
				g_cell_number_t[g_group][2]=(g_mboxData[boxNumber][3]&0xf0)>>4;

				g_cell_number_v[g_group][3]=g_mboxData[boxNumber][4]&0x0f;
				g_cell_number_t[g_group][3]=(g_mboxData[boxNumber][4]&0xf0)>>4;

				g_cell_number_v[g_group][4]=g_mboxData[boxNumber][5]&0x0f;	
				g_cell_number_t[g_group][4]=(g_mboxData[boxNumber][5]&0xf0)>>4;	
																	
			    break;
            case 0x018FF14://���������Ϣ3��6802�ĸ�������ز�ѹ����������������CC3
				break;
		    case 0x018FF16:   //�ж������ź� �Ƿ������һ������
                switch(g_group) 
                {
                    case 0:
                      g_circleFlag |= 0x00000001;
                      break;
                    case 1:
                      g_circleFlag |= 0x00000002;
                      break;
                    case 2:
                      g_circleFlag |= 0x00000004;
                      break;
                    case 3:
                      g_circleFlag |= 0x00000008;
                      break;
                    case 4:
                      g_circleFlag |= 0x00000010;
                      break;
                    case 5:
                      g_circleFlag |= 0x00000020;
                      break;
                    case 6:
                      g_circleFlag |= 0x00000040;
                      break;
                    case 7:
                      g_circleFlag |= 0x00000080;
                      break;
                    case 8:
                      g_circleFlag |= 0x00000100;
                      break;
                    case 9:
                      g_circleFlag |= 0x00000200;
                      break;
                    case 10:
                      g_circleFlag |= 0x00000400;
                      break;
                    case 11:
                      g_circleFlag |= 0x00000800;
                      break;
                    case 12:
                      g_circleFlag |= 0x00001000;
                      break;
                    case 13:
                      g_circleFlag |= 0x00002000;
                      break;
                    case 14:
                      g_circleFlag |= 0x00004000;
                      break;
                    case 15:
                      g_circleFlag |= 0x00008000;
                      break;
                    case 16:
                      g_circleFlag |= 0x00010000;
                      break;
                    case 17:
                      g_circleFlag |= 0x00020000;
                      break;      				                				          
                }
                //State_Box_Online=g_circleFlag;
                break;
            //******group#1*******
            //********************
            case 0x0018FF21://��ţ�1����صĲ�ѹֵ��1���ɲɼ�4·��ѹ:
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][0][i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF22://��ţ�1����صĲ�ѹֵ��2���ɲɼ�4·��ѹ:
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][0][4+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF23://��ţ�1����صĲ�ѹֵ��3���ɲɼ�4·��ѹ��  				  
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][0][8+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                } 
                break;  			  		
            case 0x0018FF37://��ţ�1����ص�Ƿѹ��ѹ��־
            	//g_groupUVflag[g_group][0] = g_mboxData[boxNumber][1]|(unsigned int)g_mboxData[boxNumber][0]<<8;
            	//g_groupOVflag[g_group][0] = g_mboxData[boxNumber][3]|(unsigned int)g_mboxData[boxNumber][2]<<8;
            	break;  				
            case 0x0018FF41://��ţ�1����صĲ���ֵ��1����ɼ?·�¶ȣ�
            	for(i=0;i<2;i++)
            	    g_singleCellTemperature[g_group][0][i]= g_mboxData[boxNumber][i];
            	break; 	
            //******** group#2**********
            //**************************
            case 0x0018FF24://��ţ�2����صĲ�ѹֵ��1���ɲɼ�4·��ѹ��
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][1][i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF25://��ţ�2����صĲ�ѹֵ��2���ɲɼ�4·��ѹ��
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][1][4+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF26://��ţ�2����صĲ�ѹֵ��3���ɲɼ�4·��ѹ��
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][1][8+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;  					
            case 0x0018FF38://��ţ�2����ص�Ƿѹ��ѹ��־
            	//g_groupUVflag[g_group][1] = g_mboxData[boxNumber][1]|(unsigned int)g_mboxData[boxNumber][0]<<8;
            	//g_groupOVflag[g_group][1] = g_mboxData[boxNumber][3]|(unsigned int)g_mboxData[boxNumber][2]<<8;
            	break;     			  
            case 0x0018FF42://��ţ�2����صĲ���ֵ��1���ɲɼ�8·�¶ȣ�  				 
                for(i=0;i<2;i++){
                    g_singleCellTemperature[g_group][1][i]= g_mboxData[boxNumber][i];
                }
                break;
            //******group#3********
            //*********************
            case 0x0018FF27://��ţ�3����صĲ�ѹֵ��1���ɲɼ�4·��ѹ��
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][2][i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF28://��ţ�3����صĲ�ѹֵ��2���ɲɼ�4·��ѹ��
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][2][4+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF29://��ţ�3����صĲ�ѹֵ��3���ɲɼ�4·��ѹ��
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][2][8+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF39://��ţ�3����ص�Ƿѹ��ѹ��־
                //g_groupUVflag[g_group][2] = g_mboxData[boxNumber][1]|(unsigned int)g_mboxData[boxNumber][0]<<8;
                //g_groupOVflag[g_group][2] = g_mboxData[boxNumber][3]|(unsigned int)g_mboxData[boxNumber][2]<<8;
                break; 					
            case 0x0018FF43://��ţ�3����صĲ���ֵ��1����ɼ?·�¶ȣ�  				
                for(i=0;i<2;i++){
                    g_singleCellTemperature[g_group][2][i]= g_mboxData[boxNumber][i];
                }
                break;  	
            //*******group#4*******
            //*********************
            case 0x0018FF2a://��ţ�4����صĲ�ѹֵ��1���ɲɼ�4·��ѹ��
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][3][i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF2b://��ţ�4����صĲ�ѹֵ��2���ɲɼ�4·��ѹ��
                for(i=0;i<4;i++)
                {
					g_singleCellVoltage[g_group][3][4+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF2c://��ţ�4����صĲ�ѹֵ��3���ɲɼ�4·��ѹ�� 
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][3][8+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF3a://��ţ�4����ص�Ƿѹ��ѹ��־  				  
            	//g_groupUVflag[g_group][3] = g_mboxData[boxNumber][1]|(unsigned int)g_mboxData[boxNumber][0]<<8;
            	//g_groupOVflag[g_group][3] = g_mboxData[boxNumber][3]|(unsigned int)g_mboxData[boxNumber][2]<<8;
            	break;  				
            case 0x0018FF44://��ţ�4����صĲ���ֵ��1���ɲɼ�8·�¶�  				 
                for(i=0;i<2;i++){
                    g_singleCellTemperature[g_group][3][i]= g_mboxData[boxNumber][i];
                }
                break;
            //******group#5********
            //*********************
            case 0x0018FF2d://��ţ�5����صĲ�ѹֵ��1���ɲɼ�4·��ѹ��	
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][4][i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF2e://��ţ�5����صĲ�ѹֵ��2���ɲɼ�4·��ѹ��
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][4][4+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF2f://��ţ�5����صĲ�ѹֵ��3���ɲɼ�4·��ѹ��  
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][4][8+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;  					
            case 0x0018FF3b://��ţ�5����ص�Ƿѹ��ѹ��־
            	//circleFlag |= 0x10000000;
            	//g_groupUVflag[g_group][4] = g_mboxData[boxNumber][1]|(unsigned int)g_mboxData[boxNumber][0]<<8;
            	//g_groupOVflag[g_group][4] = g_mboxData[boxNumber][3]|(unsigned int)g_mboxData[boxNumber][2]<<8;
            	break;  					
            case 0x0018FF45://��ţ�5����صĲ���ֵ��1���ɲɼ�8·�¶ȣ�  			
                for(i=0;i<2;i++){
                    g_singleCellTemperature[g_group][4][i]= g_mboxData[boxNumber][i];
                }
                break;
                
            /* new add, the max and min voltage/temperature in erery pack */
		    case 0x0018FF36://��ţ�1����ѹ��ֵ   3601 ��ʾBMU1�����пڵĵ�ѹ��ֵ
                g_singleCellVmax[g_group]= g_mboxData[boxNumber][3]|(unsigned int)g_mboxData[boxNumber][2]<<8;
                g_singleCellVmin[g_group]= g_mboxData[boxNumber][5]|(unsigned int)g_mboxData[boxNumber][4]<<8;
                g_singleCellVmax_Num[g_group]= g_mboxData[boxNumber][6];
                g_singleCellVmin_Num[g_group]= g_mboxData[boxNumber][7];
                break;       	
            case 0x0018FF4B://��ţ�1���¶���ֵ       
                g_singleCellTmax[g_group]= g_mboxData[boxNumber][0];
                g_singleCellTmin[g_group]= g_mboxData[boxNumber][1];
                g_singleCellTmax_Num[g_group]= g_mboxData[boxNumber][2];
                g_singleCellTmin_Num[g_group]= g_mboxData[boxNumber][3];
                break;
            default:
                break;
		} // end switch(framID)  //��������
    }
}
//******************************************************************************
//* Function name:   HighGroup
//* Description:     ��ֵģ��λ��ȷ��
//* EntryParameter : None
//* ReturnValue    : 1���յ�������Ϣ��0��δ�յ�������Ϣ
//******************************************************************************
unsigned char ModelLocation(unsigned char Addr) 
{
    if((Addr==0)||(Addr==1))
        return 1;
    else
        return 2;
}
//******************************************************************************
//* Function name:   bmuProcess2
//* Description:     �յ����е�����Ϣ��������弫ֵ
//* EntryParameter : None
//* ReturnValue    : 1���յ�������Ϣ��0��δ�յ�������Ϣ
//******************************************************************************
unsigned char bmuProcess2(void)//
{
    U16         buf=0,Cell_V_Max=0,Cell_V_Min=0;
    U8          Cell_V_Max_Num=0,Cell_V_Min_Num=0;
    U8          Cell_V_Max_Pack_Num=0,Cell_V_Min_Pack_Num=0;
    
    U8          Cell_T_Max=0,Cell_T_Min=0;
    U8          Cell_T_Max_Num=0,Cell_T_Min_Num=0;  
    U8          Cell_T_Max_Pack_Num=0,Cell_T_Min_Pack_Num=0;
     
    U16         i,j,k,h,t,boxNumber=0,count=0;
    U32         sum=0;
    U16         cell[650];
	U8			temperature[54];
    U8          ti=0,ci=0;
    U8          HighBMUAddr=0,HighBMUGroupNum=0,HighBMUNum=0;
    U8          LowBMUAddr=0,LowBMUGroupNum=0,LowBMUNum=0;
    static U8   ErrorBMUAddr=0;
    static U8   ErrorBMUGroup=0;
    static U8   ErrorBMULocation=0;
    static U8   TemLossState=0;
    static U8   TemLossTime=0;
    U8          cnt = 0;
    U8			pack_cnt = 0;
    U8			cell_cnt = 0;
    
    for(i=0;i<650;i++)
    {
        cell[i]=0;
    }	

    if((g_circleFlag==G_BMU_CIRCLE_FLAG)&&(g_configFlag==G_BMU_CIRCLE_FLAG))//����յ����еı��ģ�����    
    {    
        g_circleFlag=0; //������Ϣ1���Ӳŷ�һ�Σ����Բ�������������ı�־λ����� 		  

        //�����ص�����ߺ���͵�ѹ,�¶�                                          
        //***�����ѹ��ֵ����***********************************************************************************		      			
        Cell_V_Max = g_singleCellVmax[0];
        Cell_V_Min = g_singleCellVmin[0];
        Cell_V_Max_Num = g_singleCellVmax_Num[0];
        Cell_V_Min_Num = g_singleCellVmin_Num[0];
        
        for(cnt=0;cnt<BMU_NUMBER;cnt++)
        {    						      						     						    
            if(g_singleCellVmax[cnt] >= Cell_V_Max)
            {
                Cell_V_Max = g_singleCellVmax[cnt];
                Cell_V_Max_Num = g_singleCellVmax_Num[cnt];
                Cell_V_Max_Pack_Num = cnt;
            }
            if(g_singleCellVmin[cnt] <= Cell_V_Min)
            {
                Cell_V_Min = g_singleCellVmin[cnt];
                Cell_V_Min_Num = g_singleCellVmin_Num[cnt];
                Cell_V_Min_Pack_Num = cnt;
            }
            
            /* the max and min cell voltage store array */
            g_storageSysVariableCell[cnt*2]       = g_singleCellVmax[cnt];
            g_storageSysVariableCell[cnt*2 + 1]   = g_singleCellVmin[cnt];
        }

        g_highestCellVoltage        = (float)Cell_V_Max / 10000;
        g_lowestCellVoltage         = (float)Cell_V_Min / 10000; 
        g_highestCellVoltage_Num    = Cell_V_Max_Num;
        g_lowestCellVoltage_Num     = Cell_V_Min_Num;     
               
        //***�����¶ȼ�ֵ����***********************************************************************************		      			
        Cell_T_Max = g_singleCellTmax[0];
        Cell_T_Min = g_singleCellTmin[0];
        Cell_T_Max_Num = g_singleCellTmax_Num[0];
        Cell_T_Min_Num = g_singleCellTmin_Num[0];

        for(cnt=0;cnt<BMU_NUMBER;cnt++)
        {    						
            if(g_singleCellTmax[cnt] >= Cell_T_Max)
            {
                Cell_T_Max = g_singleCellTmax[cnt];
                Cell_T_Max_Num = g_singleCellTmax_Num[cnt];
                Cell_T_Max_Pack_Num = cnt;
            }
            if(g_singleCellTmin[cnt] <= Cell_T_Min)
            {
                Cell_T_Min = g_singleCellTmin[cnt];
                Cell_T_Min_Num = g_singleCellTmin_Num[cnt];
                Cell_T_Min_Pack_Num = cnt;
            }
            
            sum += (U32)(g_singleCellTmax[cnt] + g_singleCellTmin[cnt]);
            
            /* the max and min cell temperature store array */
            g_storageSysVariableT[cnt*2]     = g_singleCellTmax[cnt] & 0x00ff;
            g_storageSysVariableT[cnt*2 + 1] = g_singleCellTmin[cnt] & 0x00ff;
        } 			

        g_lowestTemperature = Cell_T_Min;
        g_highestTemperature = Cell_T_Max;   //ע���¶���48��ƫ����
        g_lowestTemperature_Num = Cell_T_Min_Num;
        g_highestTemperature_Num = Cell_T_Max_Num;   //ע���¶���48��ƫ����
        
        sum = sum - g_highestTemperature - g_lowestTemperature;
        Tavg = (U8)(sum / ((U32)BMU_NUMBER*2 - 2));
        g_averageTemperature = Tavg;
        
        //�������¶����·���5�顣��ת����char����
		count = 0;
		for(i=0;i<BMU_NUMBER;i++){
			for(j=0;j<g_bmu2_number_t[i];j++){
				for(k=0;k<g_cell_number_t[i][j];k++){
					//g_cellTemperature[i][j] = g_singleCellTemperature[i][j][k]&0x00ff;
					temperature[count] = g_singleCellTemperature[i][j][k] & 0x00ff;
					count++;
				}
			}
		}
		
        ti = 0;
        for(i=0;i<CELL_TEMP_GROUP;i++)
        {
            for(j=0;j<6;j++)
            {
            	if(ti < count){
            	    g_cellTemperature[i][j] = (U8)(temperature[ti]);//�ֱ���Ϊ-50���Դ˴�+10
            	}
            	else{
            	    g_cellTemperature[i][j] = 0;
            	}
            	ti++;
            }
        }
        	
        //�������ѹ���·��飬45�顣��ת����char����
		h = 0;
		for(i=0;i<BMU_NUMBER;i++){
			for(j=0;j<g_bmu2_number_v[i];j++){
				for(k=0;k<g_cell_number_v[i][j];k++){
					cell[h] = g_singleCellVoltage[i][j][k];
					h++;
				}
			}
		}

        ci = 0;
        for(i=0;i<CELL_VOL_GROUP;i++)//
        {
        	for(j=0;j<3;j++)
        	{
        		if(ci<h)
        		{   
        		    g_cellVol[i][2*j] = (U8)(cell[ci]); 
        		    g_cellVol[i][2*j+1] = (cell[ci])>>8;
        		    
        		}
        		else
        		{    					  
        		    g_cellVol[i][2*j] = 0;
        		    g_cellVol[i][2*j+1] = 0;
        		}
        		ci++;
        	}
        }
        //************************************************************************************************
        for(i=0;i<1;i++)  //��ս��ջ���
        {
            g_mboxID=0;
            for(j=0;j<8;j++){
                g_mboxData[0][j] = 0;
            }
        }
        
        if((g_lowestCellVoltage==0)||(g_lowestTemperature==0)||(TemLossState==1))              
            return 0;
        else 
            return 1;	 //ȫ�����ݴ������
  	} // end of circleflag	
    return 0;
}

//**********************************************************************
//**********************************************************************
