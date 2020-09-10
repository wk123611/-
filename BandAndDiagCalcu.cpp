//�����Ԫ��λ�����󣬾Ϳ����������һάѹ���洢ʱ�����а�����Խ�Ԫ��ַ��
#include "Main.h"


void BandAndDiagCalcu(int nTotalElem, int nTotalDOF, Element* pElem, int** pElemDOF, int* pDiag)
{
	int iMinDOF;
	int iBuf;
	int iDOFIndex;
	int i,j;

	for (i=0;i<nTotalDOF;i++)    //���а������1
		pDiag[i] = 1;
	for (i=0;i<nTotalElem;i++)
	{
		iMinDOF = pElemDOF[i][0];
		if ((pElem+i)->iType==TRUSS)
		{
			for (j=0;j<4;j++)       //����ܵ�Ԫ��4���ڵ�λ�Ʊ��ѡ����С��
			{
				if (pElemDOF[i][j] < iMinDOF)
					iMinDOF = pElemDOF[i][j];
			}
		}
		else
		{
			for (j=0;j<6;j++)      //�Ӹռܵ�Ԫ��6���ڵ�λ�Ʊ��ѡ����С��
			{
				if (pElemDOF[i][j] < iMinDOF)
					iMinDOF = pElemDOF[i][j];
			}
		}
		if ((pElem+i)->iType==TRUSS)
		{
			for (j=0;j<4;j++)
			{
				iDOFIndex = pElemDOF[i][j];
				iBuf = iDOFIndex - iMinDOF + 1;        //��������

				if (iBuf > pDiag[iDOFIndex])
					pDiag[iDOFIndex] = iBuf;
			}
		}
		else
		{
			for (j=0;j<6;j++)
			{
				iDOFIndex = pElemDOF[i][j];
				iBuf = iDOFIndex - iMinDOF + 1;        //��������

				if (iBuf > pDiag[iDOFIndex] && iBuf <= iDOFIndex + 1)
					pDiag[iDOFIndex] = iBuf;
				else if (iBuf > iDOFIndex + 1)
					pDiag[iDOFIndex] = iDOFIndex + 1;
			}
		}
	}
	pDiag[0] = 0;
	for (i = 1; i < nTotalDOF; i++)
	{
		pDiag[i] = pDiag[i] + pDiag[i - 1];
	}
}