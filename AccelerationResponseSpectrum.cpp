//���չ�·����������ƹ淶JTG T2231 - 01 - 2020��Ӧ�׼��������

#include "Main.h"
using namespace std;


void AccelerationResponseSpectrum(int EffectiveDOF1,double* T,double** FF,double** SN,ARS* pARS)
{
	int I;
	int J;
	int N = EffectiveDOF1 - 1;
	//��·����������ƹ淶JTG T2231 - 01 - 2020��Ӧ�׼���//////////////////
	//���룺1��������� 2����������Tg 3����������Ҷ� 4��ˮƽor���� 5��Ci��Cd��T0
	//�����Cs��A��Smax��S��T��
	// --------------------------------------------------------------------------------------------------
	int cdlx ;    ////////////////////////����������0~4����
	char ld;    ////////////////////////��������Ҷ���6~9����	
					  //A=0.05,B=0.1,C=0.15,D=0.2,E=0.3,F=0.4
	char Tg1 ;  //////////////////////�����й����𶯲�����������ȡ
					 //A=0.35,B=0.4,C=0.45				
	double A ; //���������ֵ���ٶ�
//	double lx ;   //////////////////////////ˮƽ��������Ϊlx = 1����������Ϊlx = 2
	double Ci ; //������Ҫ��ϵ�������淶3.1.3 - 2ȡֵ
	double Cd ; //�������ϵ��,��5.2.4ȡֵ��ȡ1��
	double T0 ; //��Ӧ�������ε�������ڣ�ȡ0.1��
	double Tg;//�������ڣ�0.35\0.40\0.45
	double Cs;//   ����ϵ��
	double Cz;//�ۺ�Ӱ��ϵ������������ṹ�������Ա��ζԵ�����ص��ۼ���
	double Smax;
	vector <double> S(EffectiveDOF1);    //����Ӧ�׼���õ��ļ��ٶ�

	cdlx = pARS->cdlx;
	ld = pARS->ld;
	Tg1 = pARS->Tg1;
	Ci = pARS->Ci;
	Cd = pARS->Cd;
	T0 = pARS->T0;
	Cz = pARS->Cz;
// 	cout << cdlx << endl;
// 	cout << ld << endl;
// 	cout << Tg1 << endl;
// 	cout << A << endl;
// 	cout << Ci << endl;
// 	cout << Cd << endl;
// 	cout << T0 << endl;
// 	cout << Cz << endl;
	///////////////////////////////////////////////////////////////////////////////////////////
	switch (Tg1)
	{
	case 'A':
		switch (cdlx) {
		case 0: Tg = 0.2;	break;
		case 1: Tg = 0.25;	break;
		case 2: Tg = 0.35;	break;
		case 3: Tg = 0.45;	break;
		case 4: Tg = 0.65;	break;
		}
		break;
	case 'B':
		switch (cdlx) {
		case 0: Tg = 0.25;	break;
		case 1: Tg = 0.30;	break;
		case 2: Tg = 0.40;	break;
		case 3: Tg = 0.55;	break;
		case 4: Tg = 0.75;	break;
		}
		break;
	case 'C':
		switch (cdlx) {
	case 0: Tg = 0.30;	break;
	case 1: Tg = 0.35;	break;
	case 2: Tg = 0.45;	break;
	case 3: Tg = 0.65;	break;
	case 4: Tg = 0.90;	break;
	}
			 break;
	}
	switch (ld)
	{
	case 'A':
		A = 0.05;
		switch (cdlx) {
		case 0: Cs = 0.72;	break;
		case 1: Cs = 0.80;	break;
		case 2: Cs = 1.00;	break;
		case 3: Cs = 1.30;	break;
		case 4: Cs = 1.25;	break;
		}
		break;
	case 'B':
		A = 0.1;
		switch (cdlx) {
		case 0: Cs = 0.74;	break;
		case 1: Cs = 0.82;	break;
		case 2: Cs = 1.00;	break;
		case 3: Cs = 1.25;	break;
		case 4: Cs = 1.20;	break;
		}
		break;
	case 'C':
		A = 0.15;
		switch (cdlx) {
		case 0: Cs = 0.75;	break;
		case 1: Cs = 0.83;	break;
		case 2: Cs = 1.00;	break;
		case 3: Cs = 1.15;	break;
		case 4: Cs = 1.10;	break;
		}
		break;
	case 'D':
		A = 0.2;
		switch (cdlx) {
		case 0: Cs = 0.76;	break;
		case 1: Cs = 0.85;	break;
		case 2: Cs = 1.00;	break;
		case 3: Cs = 1.00;	break;
		case 4: Cs = 1.00;	break;
		}
		break;
	case 'E':
		A = 0.3;
		switch (cdlx) {
		case 0: Cs = 0.85;	break;
		case 1: Cs = 0.95;	break;
		case 2: Cs = 1.00;	break;
		case 3: Cs = 1.00;	break;
		case 4: Cs = 0.95;	break;
		}
		break;
	case 'F':
		A = 0.4;
		switch (cdlx) {
		case 0: Cs = 0.90;	break;
		case 1: Cs = 1.00;	break;
		case 2: Cs = 1.00;	break;
		case 3: Cs = 1.00;	break;
		case 4: Cs = 0.90;	break;
		}
		break;
	}
	//���������������Ч���ٶ�
	for (I = 0; I <= N; I++)
	{
		Smax = 2.5*Ci*Cs*Cd*A;
		if (T[I] < T0)
		{
			S[I] = Smax*(0.6*T[I] / T0 + 0.4);
		}
		else if (T[I] < Tg)
		{
			S[I] = Smax;
		}
		else
		{
			S[I] = Smax*Tg / T[I];
		}	
	}


	//������ڵ������
 	for (I = 0; I <= N; I++)
	{
		for (J = 0; J <= N; J++)
		{
			FF[J][I] = Cz * S[I] *SN[J][I]*9.8/1000;
		}
	}

}
