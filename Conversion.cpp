//������ֵ��С˳��ת��Ƶ�ʣ����ڣ����;��󣬵��������������������Ͳ������������ȵȡ�
#include "Main.h"
//	������Ƶ�ʡ����ڡ���������
void Conversion(int EffectiveDOF1, double* E, double* T, double* ZXCYZLx,
	double* ZXCYZLxB, double** X, double** Fx)
{
	int N = EffectiveDOF1 - 1;
	int I, J, Q;
	for (I = 1; I <= N; I++)
	{
		for (J = I - 1; J >= 0; J--)
		{
			if (E[J + 1] < E[J])
			{
				//Ƶ�ʡ����ڡ���������
				double tempE = E[J];
				E[J] = E[J + 1];
				E[J + 1] = tempE;
				double tempT = T[J];
				T[J] = T[J + 1];
				T[J + 1] = tempT;
				for (Q = 0; Q <= N; Q++)
				{
					double tempX = X[Q][J];
					X[Q][J] = X[Q][J + 1];
					X[Q][J + 1] = tempX;
				}
			}
		}
	}
	
		for (I = 1; I <= EffectiveDOF1; I++)
		{
			for (J = I - 1; J >= 0; J--)
			{
				if (ZXCYZLx[J] < ZXCYZLx[J + 1])
				{
					//���Ͳ������������Ͳ���������������
					double tempOx = ZXCYZLx[J];
					ZXCYZLx[J] = ZXCYZLx[J + 1];
					ZXCYZLx[J + 1] = tempOx;
					double tempMPBx = ZXCYZLxB[J];
					ZXCYZLxB[J] = ZXCYZLxB[J + 1];
					ZXCYZLxB[J + 1] = tempMPBx;
					for (Q = 0; Q < EffectiveDOF1 / 3; Q++)
					{
						//����������
						double tempFx = Fx[Q][J];
						Fx[Q][J] = Fx[Q][J + 1];
						Fx[Q][J + 1] = tempFx;
					}
				}
			}
		}
	}