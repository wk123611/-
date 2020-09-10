
//�Խڵ����ɶȱ���Ժ󣬸��ݵ�Ԫ���˵Ľڵ�žͿ����γɵ�Ԫ��λ������
//��Ԫ��λ��������ڶ�ά�����У��Ա����������а����ͶԶԽ�Ԫ��ַʱʹ�á�

#include "Main.h"


void ElementDOFCalcu(int nTotalElem, Node* pNode, Element* pElem, int** pElemDOF)
{
	int iNode0, iNode1;
	int i, j;
	for (i = 0; i < nTotalElem; i++)
	{
		iNode0 = (pElem + i)->iaNode[0];
		iNode1 = (pElem + i)->iaNode[1];
		if ((pElem + i)->iType == TRUSS)   //����ܵ�Ԫ
		{
			for (j = 0; j < 2; j++)
			{
				pElemDOF[i][j] = (pNode + iNode0)->iaDOFIndex[j];
				pElemDOF[i][j+2] = (pNode + iNode1)->iaDOFIndex[j];
			}
		}
		else
		{
			for (j = 0; j < 3; j++)   //�Ըռܵ�Ԫ
			{
				pElemDOF[i][j] = (pNode + iNode0)->iaDOFIndex[j];
				pElemDOF[i][j + 3] = (pNode + iNode1)->iaDOFIndex[j];
			}
		}
	}
}
