// QR_Encode.h : CQR_Encode �N���X�錾����уC���^�[�t�F�C�X��`
// Date 2006/05/17	Ver. 1.22	Psytec Inc.
// Date 2015/11/18	fuqing change for  k21.

#ifndef __QR_ENCDOE_H__
#define __QR_ENCDOE_H__

#ifdef __cplusplus
extern "C" {
#endif

	// ���������x��
	#define QR_LEVEL_L				0
	#define QR_LEVEL_M				1
	#define QR_LEVEL_Q				2
	#define QR_LEVEL_H				3

	// �f�[�^���[�h
	#define QR_MODE_NUMERAL		0
	#define QR_MODE_ALPHABET		1
	#define QR_MODE_8BIT					2
	#define QR_MODE_KANJI				3

	// �o�[�W����(�^��)�O���[�v
	#define QR_VRESION_S					0				// 1 �` 9
	#define QR_VRESION_M				1				// 10 �` 26
	#define QR_VRESION_L					2				// 27 �` 40

#if 0	

	--<OEM Size>--
	#define MAX_ALLCODEWORD		3706			// ���R�[�h���[�h���ő�l
	#define MAX_DATACODEWORD	2956			// �f�[�^�R�[�h���[�h�ő�l(�o�[�W����40-L)
	#define MAX_CODEBLOCK			153				// �u���b�N�f�[�^�R�[�h���[�h���ő�l(�q�r�R�[�h���[�h���܂�)
	#define MAX_MODULESIZE			177				// ��Ӄ��W���[�����ő�l

#else

		// 21:Version=1,����ַ�=17(8.5������)
		// 25:Version=2,����ַ�=32(16������)
		// 29:Version=3,����ַ�=49(24.5������)
		// 33:Version=4,����ַ�=78(39������)
		// 37:Version=5,����ַ�=106(53������) 
		// 41:Version=6,����ַ�=134(67������)
		// 45:Version=7,����ַ�=154(77������)
		// 49:Version=8,����ַ�=192(96������)

	#define MAX_MODULESIZE			49				
	#define MAX_ALLCODEWORD		(MAX_MODULESIZE*20)	
	#define MAX_DATACODEWORD	(MAX_MODULESIZE*19)	
	#define MAX_CODEBLOCK			153	
		
	//106 1122222222222222222222222222222222233333333333333333333333333333333333333333333333334345444444444444444567
	//134 11222222222222222222222222222222222333333333333333333333333333333333344554445345353455333333333345435535333333334345444444444444444567

#endif

	// �r�b�g�}�b�v�`�掞�}�[�W��
	#define QR_MARGIN			4


	/////////////////////////////////////////////////////////////////////////////
	typedef struct tagRS_BLOCKINFO
	{
		int ncRSBlock;			// �q�r�u���b�N��
		int ncAllCodeWord;		// �u���b�N���R�[�h���[�h��
		int ncDataCodeu32;		// �f�[�^�R�[�h���[�h��(�R�[�h���[�h�� - �q�r�R�[�h���[�h��)

	} RS_BLOCKINFO, *LPRS_BLOCKINFO;


	/////////////////////////////////////////////////////////////////////////////
	// QR�R�[�h�o�[�W����(�^��)�֘A���

	typedef struct tagQR_VERSIONINFO
	{
		int nVersionNo;					// �o�[�W����(�^��)�ԍ�(1�`40)
		int ncAllCodeWord;				// ���R�[�h���[�h��

										// �ȉ��z��Y���͌�������(0 = L, 1 = M, 2 = Q, 3 = H) 
		int ncDataCodeu32[4];			// �f�[�^�R�[�h���[�h��(���R�[�h���[�h�� - �q�r�R�[�h���[�h��)

		int ncAlignPoint;				// �A���C�����g�p�^�[�����W��
		int nAlignPoint[6];				// �A���C�����g�p�^�[�����S���W

		RS_BLOCKINFO RS_BlockInfo1[4];	// �q�r�u���b�N���(1)
		RS_BLOCKINFO RS_BlockInfo2[4];	// �q�r�u���b�N���(2)

	} QR_VERSIONINFO, *LPQR_VERSIONINFO;


	/////////////////////////////////////////////////////////////////////////////
	// CQR_Encode �N���X
	
    typedef struct
    {
		int 	nLevel;				// ���������x��
		int 	nVersion;			// �o�[�W����(�^��)
		int 	bAutoExtent;		// �o�[�W����(�^��)�����g���w��t���O
		int 	nMaskingNo;			// �}�X�L���O�p�^�[���ԍ�
	
		int 	nSymbleSize;
		u8 		byModuleData[MAX_MODULESIZE][MAX_MODULESIZE]; // [x][y]
		// bit5:�@�\���W���[���i�}�X�L���O�ΏۊO�j�t���O
		// bit4:�@�\���W���[���`��f�[�^
		// bit1:�G���R�[�h�f�[�^
		// bit0:�}�X�N��G���R�[�h�`��f�[�^
		// 20h�Ƃ̘_���a�ɂ��@�\���W���[������A11h�Ƃ̘_���a�ɂ��`��i�ŏI�I�ɂ�BOOL�l���j
	
		int 	ncDataCodeu32Bit;					// �f�[�^�R�[�h���[�h�r�b�g��
		u8 		byDataCodeu32[MAX_DATACODEWORD];	// ���̓f�[�^�G���R�[�h�G���A
	
		int 	ncDataBlock;
		u8 		byBlockMode[MAX_DATACODEWORD];
		int 	nBlockLength[MAX_DATACODEWORD];
	
		int 	ncAllCodeWord;						// ���R�[�h���[�h��(�q�r�������f�[�^���܂�)
		u8 		byAllCodeWord[MAX_ALLCODEWORD];		// ���R�[�h���[�h�Z�o�G���A
		u8 		byRSWork[MAX_CODEBLOCK];			// �q�r�R�[�h���[�h�Z�o���[�N
    }QR_DATA;
	//extern QR_DATA 	QR_gData;

// �f�[�^�G���R�[�h�֘A�t�@���N�V����

	int EncodeData(int nLevel, int nVersion, int bAutoExtent, int nMaskingNo, const char* lpsSource, int ncSource );//<q>private:
	int GetEncodeVersion(int nVersion, const char* lpsSource, int ncLength);
	int EncodeSourceData(const char* lpsSource, int ncLength, int nVerGroup);

	int GetBitLength(u8 nMode, int ncData, int nVerGroup);

	int SetBitStream(int nIndex, u32 wData, int ncData);

	int IsNumeralData(unsigned char c);
	int IsAlphabetData(unsigned char c);
	int IsKanjiData(unsigned char c1, unsigned char c2);

	u8 AlphabetToBinaly(unsigned char c);
	u32 KanjiToBinaly(u32 wc);

	void GetRSCodeu32(u8* lpbyRSWork, int ncDataCodeu32, int ncRSCodeu32);

// ���W���[���z�u�֘A�t�@���N�V����

	//void FormatModule();

	//void SetFunctionModule();
	void SetFinderPattern(int x, int y);
	void SetAlignmentPattern(int x, int y);
	//void SetVersionPattern();
	//void SetCodeu32Pattern();
	void SetMaskingPattern(int nPatternNo);
	void SetFormatInfoPattern(int nPatternNo);
	//int  CountPenalty();



	
	/**
	 * ���ɶ�ά��ͼƬ�ļ�
	 *
	 * @param pInfo			[in]					��Ҫ���ɵ�������Ϣ����
	 * @param nLen			[in]					��Ҫ���ɵ�������Ϣ���ݳ���
	 * @param bHighQuality	[in]					�Ƿ����ɸ�������ά�룬YESΪ������Ч����NOΪ��ͨЧ��
	 * @param nScale		[in]					��ά��ͼƬ�Ŵ���
	 * @param pImgFile		[in]					��ά��ͼƬ�ļ���
	 * @param pImgWidth		[out]					��ά��ͼƬ���
	 * @retval {@link RET_OK}  						�ɹ�
	 * @retval {@link RET_ERR}						����
	 */
	extern int Lib_QrCodeImg(IMAGE *rImge,const char* pInfo ,int bHighQuality);



#ifdef __cplusplus
}
#endif

#endif
