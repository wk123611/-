#include "Main.h"

//********************��ʼ����ת�仯����Ϳ�ʼ����*******************

void RotationTransformation(double** M,double** K,int EffectiveDOF1, double* E, double* T, 
	double**X,double**Msave, double**SN,double*ZXCYXS)
{
	int NSWEEP = 0;
	double RTOL = pow(0.1, 12);//ָ������
	int NSMAX = 40000;//�������ɨ�����
	int ND = EffectiveDOF1;//�������նȾ���ά��(N DIMENSION)
	int N = ND - 1;//���������
	int NR = N - 1;//����Ԫ�ؼ��ʱ�ñ��
	double EPS = 0;//�ж��Ƿ���Ҫ����Ԫ�صĲ���
	double CA = 0; //��ת����a
	double CG = 0; //��ת����b
	int JJ,L,J,I;  //��������
	double* D = new double[ND];     
	double* EIGV = new double[ND];    //����ֵ
 	double** XT = TwoArrayDoubAlloc(ND,ND);      //���;���X��ת��
	double* ZLXL = new double[ND];      //��������					
	VectorZeroize(ND, D);
	VectorZeroize(ND, EIGV);
	VectorZeroize(ND, ZLXL);
 	MatrixZeroize(ND,ND,XT);
//��ʼ�����������----------------------------------------------------------------
	for (I = 0; I <= N; I++)
		for (J = 0; J <= N; J++)
		{
			X[I][J] = 0;
			X[I][I] = 1;
		}
	for (J = 0; J <= N; J++)
	{
		D[J] = 0.0;
	}
	for (I = 0; I <= N; I++)
	{
		if (K[I][I] > 0 && M[I][I] > 0)
		{
			D[I] = K[I][I] / M[I][I];
			EIGV[I] = D[I];
			E[I] = sqrt(EIGV[I]);
			T[I] = 2 * 3.1415926 / E[I];
		}
	}
	//������������
	for (I = 0; I <= N; I++)
	{
		for (J = 0; J <= N; J++)
		{
			ZLXL[I] = M[I][J] + ZLXL[I];
			
		}
	}

//**CHEACK IF PRESENT OFF - DIAGONAL  ELEMENT IS LARGR ENOUGH TO REQUIRE ZEROIN

part0:
	NSWEEP = NSWEEP + 1;
	EPS = pow(0.01, NSWEEP * 3);
	for (J = 0; J <= NR; J++)
	{
		JJ = J + 1;
		for (L = JJ; L <= N; L++)
		{
			double EPTOLK = (K[J][L] * K[J][L] / (K[J][J] * K[L][L]));  //�����Ԫ��K[J,L]
			double EPTOLM = (M[J][L] * M[J][L] / (M[J][J] * M[L][L]));  //�����Ԫ��M[J,L]
			if (EPTOLK >= EPS || EPTOLM >= EPS)
			{
				//���δͨ��������ת�任��������ת����CA��CG
				double KLL = K[L][L] * M[J][L] - M[L][L] * K[J][L];
				double KJJ = K[J][J] * M[J][L] - M[J][J] * K[J][L];
				double KM = K[J][J] * M[L][L] - M[J][J] * K[L][L];
				double CHECK = (KM*KM + 4 * KLL*KJJ) / 4;
				if (CHECK < 0)
				{
					cout << "check wrong" << endl;
				}
				else {
					double D1 = KM / 2 + sqrt(CHECK);
					double D2 = KM / 2 - sqrt(CHECK);
					double DEN = D1;
					if (abs(D2) > abs(D1))
					{
						DEN = D2;
					}
					if (DEN == 0)
					{
						CA = 0;
						CG = -K[J][L] / K[L][L];
					}
					else {
						CA = KLL / DEN;
						CG = -KJJ / DEN;
						//��ת����CA��CG�������
					}
				}
				///////////
				int L1 = L + 1;
				int L2 = L - 1;
				int J1 = J + 1;
				int J2 = J - 1;

				if (J > 0)
				{
					goto part1;
				}
				else
				{
					goto part2;
				}
				//PART1
			part1:
				for (int I = 0; I < J; I++)
				{
					double KJ = K[I][J];
					double MJ = M[I][J];
					double KL = K[I][L];
					double ML = M[I][L];
					K[I][J] = KJ + CG*KL;
					M[I][J] = MJ + CG*ML;
					K[I][L] = KL + CA*KJ;
					M[I][L] = ML + CA*MJ;
				}
				//PART2:
			part2:
				int JUDGE = L - N + 1;
				if (JUDGE <= 0)
				{
					for (int I = L1; I <= N; I++)
					{
						double KJ = K[J][I];
						double MJ = M[J][I];
						double KL = K[L][I];
						double ML = M[L][I];
						K[J][I] = KJ + CG*KL;
						M[J][I] = MJ + CG*ML;
						K[L][I] = KL + CA*KJ;
						M[L][I] = ML + CA*MJ;
					}
				}
				else
				{
					goto part3;

				}
				//PART3:
			part3:
				if (J1 - L2 <= 0)
				{
					for (int I = J1; I <= L2; I++)
					{
						double KJ = K[J][I];
						double MJ = M[J][I];
						double KL = K[I][L];
						double ML = M[I][L];
						K[J][I] = KJ + CG*KL;
						M[J][I] = MJ + CG*ML;
						K[I][L] = KL + CA*KJ;
						M[I][L] = ML + CA*MJ;
					}
				}
				else {
					goto part4;
				}
				//PART4:
			part4:
				double KL = K[L][L];
				double ML = M[L][L];
				K[L][L] = KL + 2 * CA*K[J][L] + CA*CA*K[J][J];
				M[L][L] = ML + 2 * CA*M[J][L] + CA*CA*M[J][J];
				K[J][J] = K[J][J] + 2 * CG*K[J][L] + CG*CG*KL;
				M[J][J] = M[J][J] + 2 * CG*M[J][L] + CG*CG*ML;
				K[J][L] = 0;
				M[J][L] = 0;
				//��ת��ɺ����X����
				for (int I = 0; I <= N; I++)
				{
					double XJ = X[I][J];
					double XL = X[I][L];
					X[I][J] = XJ + CG*XL;
					X[I][L] = XL + CA*XJ;
				}
			}
		}
	}

	//��ת��ɺ��������ֵ����
	for (int I = 0; I <= N; I++)
	{
 		if (K[I][I] > 0 && M[I][I] > 0)
		{
			EIGV[I] = K[I][I] / M[I][I];	
		}
 		else
		{
 			cout << EIGV[I] << endl;
			cout << "eigv wrong...STOP" << endl;
 		}
	}
	//�ж��Ƿ�����
	for (int I = 0; I <= N; I++)
	{
		double TOL = RTOL*D[I];
		double DIF = abs(EIGV[I] - D[I]);
		if (DIF > TOL)
		{
			for (int I = 0; I <= N; I++)
			{
				D[I] = EIGV[I];
			}
			if (NSWEEP < NSMAX)
			{
				goto part0;
			}
			else
			{
				cout << "����ʧ�ܣ�ɨ������ﵽ���" << endl;
				/*break;*/
			}
		}
		else
		{
			//���ǶԽ���Ԫ���Ƿ���Ҫ�ٴι���
			EPS = pow(RTOL, 2);
			for (int J = 0; J <= NR; J++)
			{
				JJ = J + 1;
				for (L = JJ; L <= N; L++)
				{
					double EPSK = K[J][L] * K[J][L] / (K[J][J] * K[L][L]);
					double EPSM = M[J][L] * M[J][L] / (M[J][J] * M[L][L]);
					if (EPSK < EPS&&EPSM < EPS)
					{
						//�޸ľ���������Ԫ�أ�
						for (int I = 0; I <= N; I++);
						{
							for (int J = 0; J <= N; J++)
							{
								K[J][I] = K[I][J];
								M[J][I] = M[I][J];
							}
						}
					}
					else {
						for (int I = 0; I <= N; I++)
						{
							D[I] = EIGV[I];
							if (NSWEEP < NSMAX)
							{
								goto part0;
							}
							else {
								cout << "����ʧ�ܣ�ɨ������ﵽ���" << endl;
								break;
							}
						}
					}
				}
			}
		}
	}



	for (int I = 0; I <= N; I++)
	{
		double MM = sqrt(M[I][I]);
		E[I] = sqrt(EIGV[I]);
		T[I] = 2 * 3.1415926 / E[I];
		for (int J = 0; J <= N; J++)
		{
			X[J][I] = X[J][I] / MM;
		}
	}

	MatrixTrans(ND, ND, X, XT);  //����ת��
	MatrixVectorMultiply1(ND, ND, XT, ZLXL, ZXCYXS);  //�������Ͳ���ϵ��
	MatrixMultiply(ND, ND, Msave, X, SN);   //M*X

	for (I=0;I<ND;I++)
	{
		for (J=0;J<ND;J++)
		{
			SN[J][I] = SN[J][I] * ZXCYXS[I];
		}	
	}
}



				


