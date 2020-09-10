#include "fstream"
#include"iostream"
#include "vector"
using namespace std;

#define TRUSS              0
#define FRAME              1

#define TRUSS_NODE         0
#define FRAME_NODE         1

#define DIRECT_X           0
#define DIRECT_Y           1
#define DIRECT_R           2

struct Material
{
	double dE;                //����ģ��
	double dMu;               //���ɱ�
	double dr;                //�ܶ�
};
struct Section
{
	double dA;                //��������
	double dIz;               //�������Ծ�
	//double dH;                //�����ߣ�
};
struct Node
{
	int iType;                //�ڵ�����
	double dX, dY;            //�ڵ�����
	int iaDOFIndex[3];     //�ڵ����ɶȱ��
};
struct Element
{
	int iType;                //��Ԫ���ͺ�
	int iaNode[2];         //��Ԫ���˽ڵ���
	int iSection;          //��Ԫ����������
	int iMaterial;         //��Ԫ����������
	double dLength;         //��Ԫ����
	double dSin, dCos;      //��Ԫ�ֲ�����x��������������x��ļнǵ�������
	double daEndInterForce;  //��Ԫ�˶�������
};
struct ConstrainedNode
{
	int iNode;                   //��Լ���ڵ��
	int iaConstrainedDOF[3];       //��Լ���ڵ����ɶ����ԣ�0-λ��λ�ã�-1λ����֪
};


struct ARS
{
	int cdlx;      ////////////////////////����������0~4����
	char ld;       //////////////////////////��������Ҷ���6~9����	
 				  //A=0.05,B=0.1,C=0.15,D=0.2,E=0.3,F=0.4
    char Tg1;  //////////////////////�����й����𶯲�����������ȡ
 				 //A=0.35,B=0.4,C=0.45	
    double Ci; //������Ҫ��ϵ�������淶3.1.3 - 2ȡֵ
	double Cd = 1.0; //�������ϵ��
	double T0 = 0.1; //��Ӧ�������ε��������
	double Tg;//��������0.35\0.40\0.45
	double Cs;// ����ϵ��
    double Cz ;//�ۺ�Ӱ��ϵ������������ṹ�������Ա��ζԵ�����ص��ۼ���

};


int DOFIndexCalcu(int& iBuf0, int nTotalNode, int nConstrainedNode, ConstrainedNode* pConsNode, Node* pNode);
int EffectiveDOF(int& iBuf0, int nTotalNode, int nConstrainedNode, ConstrainedNode* pConsNode, Node* pNode);
void ElementDOFCalcu(int nTotalElem, Node* pNode, Element* pElem, int** pElemDOF);
void BandAndDiagCalcu(int nTotalElem, int nTotalDOF, Element* pElem, int** pElemDOF, int* pDiag);
double** TwoArrayDoubAlloc(int nRow, int nCol);
int ** TwoArrayIntbAlloc(int nRow, int nCol);
template<class T>
void TwoArrayFree(int nRow, T** pdi) 
{
	for (int i = 0; i < nRow; i++)
		delete[]pdi[i];   //�����пռ�	
	delete[]pdi;      //�����пռ�

};
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
};
template<class T>
void VectorZeroize(int n, T* pT)
{
	for (int i = 0; i < n; i++)
		pT[i] = 0;
};
void MatrixMultiply(int nRow, int nCol, double** pA, double** pB, double** PC);
void MatrixVectorMultiply1(int nRow, int nCol, double** pA, double* pB, double* pC);
void MatrixVectorMultiply2(int nRow, int nCol, double** pA, double* pB, double* pC);
void MatrixTrans(int nRow, int nCol, double** pA, double** pAT);
void LengthSinCosCalcu(int nTotalElem, Element* pElem, Node*  pNode);
void TrussElemStiffcalcu(ofstream&fout1, Element* pElem, Material* pMate, Section * pSect, double** pKe);
void FrameElemStiffCalcu(ofstream&fout1, Element* pElem, Material* pMate, Section* pSect, double** pKe);
void GKAssembly(int nToallDOF, int nToallElem, Element* pElem, Node* pNode, Material* pMate, Section* pSect, int* pDiag, double* pGK);
void GetK(int iBuf, int* pDiag, double* pGK, int* BDK, double** K);
void GetM(int iBuf, int* pDiag, double* pGM, int* BDK, double** M, double** Msave);
void FrameElemMassCalcu(ofstream&fout1, Element* pElem, Material* pMate, Section* pSect, double** pMe);
void GMAssembly(int nToallDOF, int nToallElem, Element* pElem, Node* pNode, Material* pMate, Section* pSect, int* pDiag, double* pGM);
void RotationTransformation(double** M, double** K, int EffectiveDOF1, double* E, double* T,
	double**X, double**Msave, double**SN, double*ZXCYXS);
void AccelerationResponseSpectrum(int EffectiveDOF1, double* T, double** FF, double** SN, ARS* pARS);
void ExtractX(int EffectiveDOF1, double** SN, double** SNx, double* ZXCYZLx,
	double* ZXCYZLB, double** F, double** Fx);
void Conversion(int EffectiveDOF1, double* E, double* T, double* ZXCYZLx,
	double* ZXCYZLxB, double** X, double** Fx);








