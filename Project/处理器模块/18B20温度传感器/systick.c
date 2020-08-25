/****************************************************************************
*
* �ļ���: sysytick.c
* ���ݼ���: ��ģ�����systick�жϵ����ã��Լ������ʱ����ʵ�֡�
*
*
*/

#include "stm32f2xx.h"
#include <stdio.h>

#include "systick.h"

/* �ڴ˶������ɸ������ʱ��ȫ�ֱ���
ע�⣬��������volatile����Ϊ����������жϺ���������ͬʱ
�����ʣ��п�����ɱ����������Ż�
*/
#define TMR_COUNT	4		/* �����ʱ���ĸ��� */
SOFT_TMR g_Tmr[TMR_COUNT];

/*******************************************************************************
	��������SoftTimerDec
	��  ��: ��ʱ������ָ��,ÿ��1ms��1
	��  ��:
	����˵����
*/
static void SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->flag == 0)
	{
		if (_tmr->count > 0)
		{
			if (--_tmr->count == 0)
			{
				_tmr->flag = 1;
			}
		}
	}
}

/*******************************************************************************
	��������SysTick_ISR
	��  ��:
	��  ��:
	����˵����SysTick�жϷ������ÿ��1ms����1��
*/
extern void AdcPro(void);
void SysTick_ISR(void)
{
	static uint8_t s_count = 0;
	uint8_t i;

	for (i = 0; i < TMR_COUNT; i++)
	{
		SoftTimerDec(&g_Tmr[i]);
	}

	if (++s_count >= 10)
	{
		s_count = 0;

		AdcPro();		/* �ú����� main.c */
	}
}

/*******************************************************************************
	��������StartTimer
	��  ��: ��ʱ��ID (0 - 3)
	��  ��:
	����˵����
*/
void StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		return;
	}

	g_Tmr[_id].count = _period;
	g_Tmr[_id].flag = 0;
}

/*******************************************************************************
	��������StartTimer
	��  ��: ��ʱ��ID (0 - 3)
	��  ��: ���� 0 ��ʾ��ʱδ���� 1��ʾ��ʱ��
	����˵����
*/
uint8_t CheckTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	return g_Tmr[_id].flag;
}

