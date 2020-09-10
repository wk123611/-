#include "Main.h"
#include "iostream"
using namespace std;
//����˫���ȶ�ά����==============================================================
double** TwoArrayDoubAlloc(int nRow, int nCol)
{
	double** pd;
	pd = new double*[nRow];      //��������
	if (!pd)
	{
		cout << "�ڴ����ʧ��" << endl;
		exit(-1);
	}
	for (int j = 0; j < nRow; j++)
	{
		pd[j] = new double[nCol];          //��������
		if (!pd[j])
		{
			cout << "�ڴ����ʧ��" << endl;
			exit(-1);
		}
	}
	return pd;
}
//���嵥���ȶ�ά����==============================================================
int** TwoArrayIntbAlloc(int nRow, int nCol)
{
	int** pi;
	pi = new int* [nRow];          //��������
	if (!pi)
	{
		cout << "�ڴ����ʧ��" << endl;
		exit(-1);
	}
	for (int j = 0; j < nRow; j++)
	{
		pi[j] = new int[nCol];          //��������
		if (!pi[j])
		{
			cout << "�ڴ����ʧ��" << endl;
			exit(-1);
		}
	}
	return pi;
}

//�ͷ��ڴ�ռ�==============================================================
/*template<class T>
void TwoArrayFree(int nRow, T** pdi)
{
	for (int i = 0; i < nRow; i++)
	{
		delete[]pdi[i];   //�����пռ�
		delete[]pdi;      //�����пռ�
	}
}

//��������==============================================================
template<class T>
void MatrixZeroize(int nRow, int nCol, T** pT)
{
	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			pT[i][j] = 0;
		}
	}
}

//��������==============================================================
template<class T>
void VectorZeroize(int n, T* pT)
{
	for (int i = 0; i < n; i++)
		pT[i] = 0;
}
*/
//���������==============================================================
void MatrixMultiply(int nRow, int nCol, double** pA, double** pB, double** pC)
{
	double dTemp;
	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			dTemp = 0.0;
			for (int k = 0; k < nCol; k++)
			{
				dTemp += pA[i][k] * pB[k][j];
				pC[i][j] = dTemp;
			}
		}
	}
}

//�����������==============================================================
void MatrixVectorMultiply1(int nRow, int nCol, double** pA, double* pB, double* pC)
{
	double dTemp;
	for (int i = 0; i < nRow; i++)
	{
		dTemp = 0.0;
		for (int j = 0; j < nCol; j++)
		{
			dTemp += pA[i][j] * pB[j];
			pC[i] = dTemp;
		}
	}
}
//�����ҳ�����
void MatrixVectorMultiply2(int nRow, int nCol, double** pA, double* pB, double* pC)
{
	double dTemp;
	for (int i = 0; i < nRow; i++)
	{
		dTemp = 0.0;
		for (int j = 0; j < nCol; j++)
		{
			dTemp += pA[j][i] * pB[j];
			pC[i] = dTemp;
		}
	}
}

//����ת��==============================================================
void MatrixTrans(int nRow, int nCol, double** pA, double** pAT)
{
	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			pAT[j][i] = pA[i][j];
		}
	}
}
