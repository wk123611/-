
#include "iostream"
#include "Main.h"
#include "fstream"
#include "stdlib.h"  //
#include "stdio.h"
#include "math.h"

using namespace std;

int main() {



	int nTotalNode;                //�ڵ�����
	int nConstrainedNode;           //��Լ���ڵ�����
	int nTotalElem;                 //��Ԫ����
	int EffectiveDOF1;              //���ɽڵ����ɶ��������л��к����ά����
	int EffectiveDOF2;
	int nMaterialType;             //����������
	int nSectionType;             //���漯������������
	int nTotalDOF;               //�����ɶ���
	int nFreeDOF;                //�������ɶ���

	int i;                        //ѭ�����Ʊ���
	int iBuf;


	ifstream fin0("INPUT.txt");
	if (!fin0)
	{
		cout << "ԭʼ�����ļ���ʧ��" << endl;
		exit(-1);
	}
	ofstream fout0("Results.dat");
	if (!fout0)
	{
		cout << "����ṹ����ļ���ʧ��" << endl;
		exit(-1);
	}

	fin0 >> nTotalNode >> nConstrainedNode >> nTotalElem >> nMaterialType >> nSectionType; //���� >> nLoad
	//�ڴ����----------------------------------------------------------------------------------------
	Node* pNode = new Node[nTotalNode];
	ConstrainedNode* pConsNode = new ConstrainedNode[nConstrainedNode];
	Element* pElem = new Element[nTotalElem];
	Material* pMate = new Material[nMaterialType];
	Section* pSect = new Section[nSectionType];
	ARS* pARS = new ARS[1];
	int** pElemDOF = TwoArrayIntbAlloc(nTotalElem, 6);
	//---------------------------------------------------------------------------------------------
	//����ṹ��������
	for (i = 0; i < nTotalNode; i++)     //����ڵ�����
		fin0 >> (pNode + i)->iType >> (pNode + i)->dX >> (pNode + i)->dY;
	for (i = 0; i < nConstrainedNode; i++)  //������Լ���ڵ�����
		fin0 >> (pConsNode + i)->iNode >> (pConsNode + i)->iaConstrainedDOF[0] >> (pConsNode + i)->iaConstrainedDOF[1] >> (pConsNode + i)->iaConstrainedDOF[2];
	for (i = 0; i < nTotalElem; i++)  //���뵥Ԫ����
		fin0 >> (pElem + i)->iType >> (pElem + i)->iaNode[0] >> (pElem + i)->iaNode[1]
		>> (pElem + i)->iSection >> (pElem + i)->iMaterial;
	for (i = 0; i < nMaterialType; i++)  //�����������
		fin0 >> (pMate + i)->dE >> (pMate + i)->dMu >> (pMate + i)->dr;
	for (i = 0; i < nSectionType; i++)  //�����������
		fin0 >> (pSect + i)->dA >> (pSect + i)->dIz ;          
	
	fin0 >> pARS->cdlx >> pARS->ld >> pARS->Tg1 >> pARS->Ci >> pARS->Cd >> pARS->Cz;
	//�������������غ�����
	//-------------------------------------------------------------------------------------------
	//���ԭʼ����
	//-------------------------------------------------------------------------------------------

	LengthSinCosCalcu(nTotalElem, pElem, pNode);
	//���������ɶ��� �ڵ����ɶȺ͵�Ԫ��λ����---------------------------------------------------
	EffectiveDOF1 = EffectiveDOF(nFreeDOF, nTotalNode, nConstrainedNode, pConsNode, pNode);
	EffectiveDOF2 = EffectiveDOF1 / 3;
	nTotalDOF = DOFIndexCalcu(nFreeDOF, nTotalNode, nConstrainedNode, pConsNode, pNode);   //�����ɶ���
	ElementDOFCalcu(nTotalElem, pNode, pElem, pElemDOF);
	
	//--------------------------------------------------------------------------------------------
	int* pDiag = new int[nTotalDOF];           //�����Ԫ��ַ
	BandAndDiagCalcu(nTotalElem, nTotalDOF, pElem, pElemDOF, pDiag);    //�����������Ԫ��ַ
	TwoArrayFree(nTotalElem, pElemDOF);        //�ͷŵ�Ԫ��λ����������ڴ�
	iBuf = pDiag[nTotalDOF - 1] + 1;           //�������Ԫ������
	double* pGK = new double[iBuf];            //һά��������ܸնȾ���������ǲ���
	double* pGM = new double[iBuf];            //һά�����������������������ǲ���
	VectorZeroize(iBuf, pGK);                  //�ܸ�����		
	VectorZeroize(iBuf, pGM);                  //������������
	GKAssembly(nTotalDOF, nTotalElem, pElem, pNode, pMate, pSect, pDiag, pGK);    //��װ�ܸ�
	GMAssembly(nTotalDOF, nTotalElem, pElem, pNode, pMate, pSect, pDiag, pGM);    //��װ�ܸ�
	double** K = TwoArrayDoubAlloc( nTotalDOF, nTotalDOF);   //�ܸնȾ���
	double** M = TwoArrayDoubAlloc(nTotalDOF, nTotalDOF);    //����������
	double** Msave = TwoArrayDoubAlloc(nTotalDOF, nTotalDOF);    //��������������д洢
	int* BDK = new int[nTotalDOF];            //���������
	VectorZeroize(nTotalDOF, BDK);
	GetK(EffectiveDOF1, pDiag, pGK, BDK, K);  
	GetM(EffectiveDOF1, pDiag, pGM, BDK,M,Msave);
	double* E = new double[EffectiveDOF1];            //����ֵ����>>ԲƵ��
	double* T = new double[EffectiveDOF1];            //����
	double* ZLXL = new double[EffectiveDOF1];         //��������
	double* ZXCYXS = new double[EffectiveDOF1];       //���Ͳ���ϵ��
	double* ZXCYZLx = new double[EffectiveDOF1];      //X�������Ͳ�������
	double* ZXCYZLxB = new double[EffectiveDOF1];      //X�������Ͳ���������
	double** X = TwoArrayDoubAlloc(EffectiveDOF1, EffectiveDOF1);    //���;���
	double** SN = TwoArrayDoubAlloc(EffectiveDOF1, EffectiveDOF1);   //��������*���;���*ÿ�����Ͳ���ϵ��
	double** SNx = TwoArrayDoubAlloc(EffectiveDOF2, EffectiveDOF1);  //��SN����ȡX�������ɶ�����
	double** F = TwoArrayDoubAlloc(EffectiveDOF1, EffectiveDOF1);    //��Ч������
	double** Fx = TwoArrayDoubAlloc(EffectiveDOF2, EffectiveDOF1);   //˳���������
	VectorZeroize(EffectiveDOF1, ZXCYXS);
	VectorZeroize(EffectiveDOF1, E);
	VectorZeroize(EffectiveDOF1, T);
	VectorZeroize(EffectiveDOF1, ZLXL);
	VectorZeroize(EffectiveDOF1, ZXCYZLx);
	VectorZeroize(EffectiveDOF1, ZXCYZLxB);
	MatrixZeroize(EffectiveDOF1, EffectiveDOF1, X);
	MatrixZeroize(EffectiveDOF1, EffectiveDOF1, F);
	MatrixZeroize(EffectiveDOF1, EffectiveDOF1, SN);
	MatrixZeroize(EffectiveDOF2, EffectiveDOF1, SNx);
	MatrixZeroize(EffectiveDOF2, EffectiveDOF1, Fx);
	RotationTransformation(M, K, EffectiveDOF1, E, T,X,Msave,SN,ZXCYXS);  //��������ֵ
	AccelerationResponseSpectrum(EffectiveDOF1,T,F,SN,pARS);              //���������
	ExtractX(EffectiveDOF1,SN,SNx,ZXCYZLx,ZXCYZLxB,F, Fx);                //��ȡ˳������    
	Conversion(EffectiveDOF1, E, T, ZXCYZLx, ZXCYZLxB, X, Fx);            //ת��



/////������=========================================================================
 	ofstream mycout("OUTPUT.txt");
	mycout << "���Ƶ��(��λ��rad/sec)" << endl;
 	for (i=0;i<EffectiveDOF1;i++)
 	{
 		
 		mycout << E[i] << "\t" <<endl;
 	}
 	mycout << "�������(��λ��s)" << endl;
 	for (i = 0; i < EffectiveDOF1; i++)
 	{
 
		mycout << T[i] << "\t"<<endl;
	}

	mycout << "������;���" << endl;
	for (i = 0; i < EffectiveDOF1; i++)
	{
		for (int j = 0; j < EffectiveDOF1; j++)
		{
			mycout << X[i][j] << "      \t";
		}
		mycout << endl;
	}
 	mycout << "���˳�������Ͳ�����������λ��Kg��" << endl;
 	for (i = 0; i < EffectiveDOF1; i++)
 	{
 
 		mycout << ZXCYZLx[i] << "\t" << endl;
 	}
 	mycout << "���˳�������Ͳ�����������" << endl;
	for (i = 0; i < EffectiveDOF1; i++)
	{
 
 		mycout << ZXCYZLxB[i] << "%\t" << endl;
 	}
 
 	mycout << "���˳�����Ч����������λ��kN��" << endl;
 	for (i=0;i<EffectiveDOF2;i++)
 	{
 		for (int j=0;j<EffectiveDOF1;j++)
 		{
 			mycout << Fx[i][j] << "      \t";
 		}
 		mycout << endl;
 	}

	cout << "��鿴����ļ�OUTPUT" << endl;
	system("pause");
	return 0;
}

