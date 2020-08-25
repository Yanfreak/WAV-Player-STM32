/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� ��sensor_data.h
*   ժ Ҫ    �� �¶ȴ������ֶȱ�
*
*   ��ǰ�汾 �� 
*   �� ��    �� EFLAG
*   ������� �� 
*
*   ȡ���汾 ��
*   ԭ����   �� 
*   �޸����� ��
*   ������� ��2012.08.01 
*
******************************************************************************/

#ifndef SENSOR_DATA_HEADER_FILE
#define SENSOR_DATA_HEADER_FILE

typedef struct {			  /* ���ݷֶȱ��ѯ�ṹ�� */
	float nIndexLow;		  /* ���ݷֶȱ��ѯ�������ݶ�Ӧ����Сֵ */
	float nIndexHigh;	      /* ���ݷֶȱ��ѯ�������ݶ�Ӧ�����ֵ */
	float nIndexFactor;		  /* ���ݷֶȱ��ѯ����������ʵ�ʷֶȵı���ϵ�� */
	int nTableSize;			  /* �ֶȱ��С */
	const float *pTableData;  /* ����ֶȱ����ݵ�����ָ�� */
}TIndexTable;

#ifndef SENSOR_DATA_FILE
extern const float TableData_NTC3950[];	/* NTC3950 ����/�¶ȷֶȱ�*/
extern TIndexTable TIndexTab_NTC3950; /* NTC3950 ����/�¶ȷֶȱ��ѯ�ṹ�� */
#endif

#endif

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����**************/
