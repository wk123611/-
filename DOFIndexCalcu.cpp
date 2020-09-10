#include "Main.h"

//�ڵ����ɶȱ�ų���


int DOFIndexCalcu(int& iBuf0, int nTotalNode, int nConstrainedNode, ConstrainedNode* pConsNode, Node* pNode)
{
	int i, j, k;
	int iBuf;     //�����ɶ���
	for (i = 0; i < nTotalNode; i++)
		for (j = 0; j < 3; j++)
			(pNode + i)->iaDOFIndex[j] = 0;   //�����ڵ����ɶȱ������
	for (i = 0; i < nConstrainedNode; i++)		
	{			
		iBuf = (pConsNode + i)->iNode;			 //��Լ���ڵ��
		for (j = 0; j < 3; j++)		           //��Լ���ڵ����������
			pNode[iBuf].iaDOFIndex[j] = (pConsNode + i)->iaConstrainedDOF[j];
	}
	iBuf = 0;
	for (i = 0; i < nTotalNode; i++)
	{
		if ((pNode + i)->iType == FRAME_NODE)
		{
			for (j = 0; j < 3; j++)         //�Ըռܽڵ�δ֪���ɶȱ��
			{
				if ((pNode + i)->iaDOFIndex[j] == 0)
					(pNode + i)->iaDOFIndex[j] = iBuf++;
			}
		}
		else
		{
			for (j=0;j<2;j++)               //����ܽڵ�δ֪���ɶȱ��
			{
				if ((pNode + i)->iaDOFIndex[j] == 0)
					(pNode + i)->iaDOFIndex[j] = iBuf++;				
			}
		}
	}
	iBuf0 = iBuf;
	for (i = 0; i < nTotalNode; i++)
	{
		if ((pNode + i)->iType == FRAME_NODE)
		{
			for (j = 0; j < 3; j++)         //�Ըռܽڵ����֪�������ɶȱ��
			{
				if ((pNode + i)->iaDOFIndex[j] == -1)
					(pNode + i)->iaDOFIndex[j] = iBuf++;
			}
		}
		else 
		{
			for (j = 0; j < 2; j++)              //����ܽڵ����֪�������ɶȱ��
			{
				if ((pNode + i)->iaDOFIndex[j] == -1)
					(pNode + i)->iaDOFIndex[j] = iBuf++;
			}
		}
	}
	for (i=0;i<nTotalNode;i++)
	{
		if ((pNode+i)->iType=FRAME_NODE)
		{
			for (j=0;j<3;j++)
			{
				if ((pNode+i)->iaDOFIndex[j]>=10000)  //�Ըռܴӽڵ����ɶȱ��
				{
					k = (pNode + i)->iaDOFIndex[j] > -10000;
					(pNode + i)->iaDOFIndex[j] = (pNode + k)->iaDOFIndex[j];
				}
			}
		}
		else
		{
			for (j=0;j<2;j++)
			{
				if ((pNode+i)->iaDOFIndex[j]>=10000)             //����ܴӽڵ����ɶȱ��
				{
					k = (pNode + i)->iaDOFIndex[j] > -10000;
					(pNode + i)->iaDOFIndex[j] = (pNode + k)->iaDOFIndex[j];
				}
			}
		}
	}
	return iBuf;	
}
