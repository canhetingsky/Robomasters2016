
#define __DRIVER_STATUSMACHINE_GLOBALS

#include "Config.h"
#include "OSinclude.h"
#include "Driver_DBUS.h"
#include "Driver_vision.h"
#include "Driver_StatusMachine.h"


/*
wasdǰ�����ҿ���
qe��������
���������䣨δ����Ħ����������Ħ���֣����Ҽ��Զ����
ctrl + shift�ر�Ħ����
*/

/**
  * @brief  ״̬����ʼ��
  * @param  void
  * @retval void
  */
void StatusMachine_InitConfig(void)
{
    AutoMode = AutoMode_OFF;
    GunStatus = GunStatus_Stop;
    ControlMode = ControlMode_Protect;
}


/**
  * @brief  ״̬��״̬����
  * @param  void
  * @retval void
  * @note   ң�������뿪��1ң�������ƣ���1������3��Ħ���֣�2����
            ���뿪��3������ģʽ�����ȫ���ر�
            ���뿪��2�����̿��ƣ��Ҳ��뿪��3��Ħ���֣�����Ҽ��Զ����
            QE����
  */
void StatusMachine_Update(void)
{
    static uint8_t RateCounter = 0;
    static uint8_t BigSampleCounter = 0;
    static uint8_t AttackCounter = 0;
    static portTickType LastPCShutdownSignalTick = 0;
    portTickType CurrentTick = xTaskGetTickCount();
    
    
    
    //֡�ʹ���ͣ��
    if(DBUSFrameRate < 3)
    {
        ControlMode = ControlMode_Protect;
        
        return;
    }
    
    //ң��������
    if(DBUS_ReceiveData.switch_left == 1)
    {
        ControlMode = ControlMode_RC;
        
        //Ħ����
        if(DBUS_ReceiveData.switch_right == 3)
        {
            GunStatus = GunStatus_Motor;
        }
        else if(DBUS_ReceiveData.switch_right == 2)
        {
            GunStatus = GunStatus_Shot;
        }
        else
        {
            GunStatus = GunStatus_Stop;
        }
        
        //�Զ�����ر�
        AutoMode = AutoMode_OFF;
    }
    //�������
    else if(DBUS_ReceiveData.switch_left == 2)
    {
        ControlMode = ControlMode_KM;
        
        //���ͷ���Ƶ�ʼ�С���ڸ���
        if(RateCounter == 4)
        {
            if(DBUS_ReceiveData.keyBoard.key_code & KEY_X)
            {
                AttackCounter = 0;
                
                if(BigSampleCounter < VisiolModeChangeDataSendNum)
                {
                    VisionType = VisionType_BigSample;
                    SendPCOrder(PCOrder_BigSample);
                    BigSampleCounter++;
                }
            }
            else
            {
                BigSampleCounter = 0;
                
                if(AttackCounter < VisiolModeChangeDataSendNum)
                {
                    VisionType = VisionType_Attack;
                    SendPCOrder(PCOrder_Attack);
                    AttackCounter++;
                }
            }
            RateCounter = 0;
        }
        else
        {
            RateCounter++;
        }
    }
    //����ģʽ
    else
    {
        ControlMode = ControlMode_Protect;
        
        //�ػ�
        if((DBUS_ReceiveData.ch1 > 600) && 
            (DBUS_ReceiveData.ch2 < -600) &&
            (DBUS_ReceiveData.ch3 < -600) &&
            (DBUS_ReceiveData.ch4 > 600))
        {
            if(LastPCShutdownSignalTick + 5000 < CurrentTick)
            {
                SendPCOrder(PCOrder_Shutdown);
            }
        }
        else
        {
            LastPCShutdownSignalTick = CurrentTick;
        }
    }
}















