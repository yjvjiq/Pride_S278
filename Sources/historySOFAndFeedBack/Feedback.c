
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : S133
//* File Name          : Feedback.c
//* Author             : judy
//* Version            : V1.0.0
//* Start Date         : 2016.2.14
//* Description        : ���ļ����ڲ��������������ǰֵ
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h"
#include"FeedbackTable.h"
//*************************************************************************
//******************************************************************************
//* Function name:   linearizationRechargePower
//* Description:     �������������ʱ��������ǰ���������������������������ʱ��S
//* EntryParameter : �¶�,SOCֵ
//* ReturnValue    : �����������س����
//******************************************************************************
float linearizationRechargePower(char tem,float soc) 
{
    float curr=0;
    unsigned char second=0;
    
    if((tem<=0)||(tem>=55)||(soc>=0.95)) 
    {
        curr=0;
        return curr;
    }
    
    if( (tem>0)&&(tem<=10))//(0,10] 
    {
        if(soc>0.8)
            curr =0;
        else if(soc>0.5) 
        {
            curr=120;
            second=5;
        } 
        else 
        {
            curr=120;
            second=10;
        }   
    }
    
    if( (tem>10)&&(tem<=20))//(10,20]
    {
        if(soc>0.8) 
        {
            curr =120;
            second=5;
        }
        else if(soc>0.5) 
        {
            curr=120;
            second=10;
        } 
        else 
        {
            curr=180;
            second=10;
        }
    }
    
    if( (tem>20)&&(tem<55)) //(20,55) 
    {
        if(soc>0.8) 
        {
            curr =120;
            second=10;
        }
        else if(soc>0.5) 
        {
            curr=180;
            second=10;
        } 
        else 
        {
            curr=240;
            second=10;
        }
    }
    
    pusleLimitSec = second;
    return curr; 
}

//******************************************************************************
//* Function name:   PulseRechargePowerAdjust
//* Description:     ����ƽ���¶ȵ�����������������������  ��VCU  ���ƶ���������
//* EntryParameter : tem, soc ƽ���¶ȣ�SOCֵ
//* ReturnValue    : fc ���صĻ�������ֵ  
//******************************************************************************
float PulseRechargePowerAdjust(char tem,float soc) 
{   
    static float current1=0; //���������ݴ�ֵ1
    static float current2=0; //���������ݴ�ֵ2;
    static unsigned int leftTime=0;
    static float fc=0;//���صĻ�������ֵ
    float BigCharCurrent = 0; 
    static unsigned char FirstTime=0;
    static unsigned char changeFlag=0;
    static unsigned int ii=0;
    static unsigned int jj=0;
    static unsigned char TimeoutFlag1=0;
    static unsigned char TimeoutFlag2=0;
    static unsigned char CloseFlag=0;
    
    //if(CloseFlag == 0)  // 
    //{ 
        BigCharCurrent = linearizationRechargePower(tem-40,soc);//��һ�ν���

        if(FirstTime==0) //��һ�ν���
        {
            fc=BigCharCurrent; 
            current1 = BigCharCurrent; 
            FirstTime=1;         
        }
        else//�ٴν���
        {
            if(changeFlag==0)//ԭֵΪ0������current2Ϊ�ڶ��ν���ú���ʱ��ֵ
                {
                    current2=BigCharCurrent;
              
                }
            if(current1!=current2) //������߲��ȣ�˵����һ��ڶ��β���
            {
                changeFlag=1;//���ñ仯��־λ
                if(current1>current2) 
                {
                    fc=current1-0.16;// ÿ20ms�仯0.5kw,7ms����һ�Σ�21ms����0.51KW
                    current1=fc;
                    if(current1<=current2)
                    {
                        
                        fc=current2;
                        changeFlag=0; 
                    }
                }
                else if(current1<current2) 
                {
                    fc=current1+0.16;// ÿ5ms�仯0.05kw
                    current1=fc;
                    if(current1>=current2) 
                    {
                        
                        fc=current2;
                        changeFlag=0;
                    }
                }
            } 
            else 
            {
                changeFlag=0; 
            }
        
        } 
    
    if(-g_systemCurrent>BigCharCurrent)//���ʵ�ʵ������ڸ��������������������������Ϊ0
        return 0;  
    //}

 /*�ȷ���  
  //if((-g_systemCurrent>=BigCharCurrent)&&(TimeoutFlag==0))
   if((-g_systemCurrent>=5)&&(TimeoutFlag1==0))//���ǳ��ַ�������
   {
      ii++;
      leftTime = pusleLimitSec*1000-7*ii;
      if(leftTime<=7)//������ʱ�䵽�ˣ�������������Ϊ0
      {
          ii = 0;
          TimeoutFlag1 = 1;
      } 
   }
   if((TimeoutFlag1 == 1)&&(TimeoutFlag2 == 0))//�������ʱ�䵽�ˣ�׼������������3�� 0ֵ
   {
      CloseFlag = 1;
      fc = 0;
      jj++;
      if(jj>=400)//��0������ʱ��Ϊ3
      {   
          TimeoutFlag2 = 1;
          jj = 0;
          CloseFlag = 0;//3���ſ��㷴����������
      } 
   }
   
 */  
   return fc; 
}


/* ��һֱ����û��ʱ�����ƣ�ʵ����������ٶ�   
    if((-g_systemCurrent<=FBpower2)&&SecondOn == 0)
        {
            jj++;
            leftTime = pusleLimitSec*1000-7*jj;
            if(leftTime<=7)//������ʱ�䵽�ˣ���׼���������𲽽��͵�0
            {
                jj = 0;
                SecondOn = 1;
            }
        } 
  
    if(SecondOn == 1)
        {
            ThirdOn = 1;
            fc = 0;
            ii++;
            if(ii>=400)//��0������ʱ��Ϊ3
            {   
                ii = 0;    
                ThirdOn = 0;
                SecondOn = 0;
            }
        }
  */

//**************************************************************************************
//**************************************************************************************
//*****************************the end*****************************************************
//*****************************************************************************************
//***************************************************************************************