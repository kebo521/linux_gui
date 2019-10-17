// QR_Encode.h : CQR_Encode 僋儔僗愰尵偍傛傃僀儞僞乕僼僃僀僗掕媊
// Date 2006/05/17	Ver. 1.22	Psytec Inc.
// Date 2015/11/18	fuqing change for  k21.

#ifndef __QR_ENCDOE_H__
#define __QR_ENCDOE_H__

#ifdef __cplusplus
extern "C" {
#endif

	// 岆傝掶惓儗儀儖
	#define QR_LEVEL_L				0
	#define QR_LEVEL_M				1
	#define QR_LEVEL_Q				2
	#define QR_LEVEL_H				3

	// 僨乕僞儌乕僪
	#define QR_MODE_NUMERAL		0
	#define QR_MODE_ALPHABET		1
	#define QR_MODE_8BIT					2
	#define QR_MODE_KANJI				3

	// 僶乕僕儑儞(宆斣)僌儖乕僾
	#define QR_VRESION_S					0				// 1 乣 9
	#define QR_VRESION_M				1				// 10 乣 26
	#define QR_VRESION_L					2				// 27 乣 40

#if 0	

	--<OEM Size>--
	#define MAX_ALLCODEWORD		3706			// 憤僐乕僪儚乕僪悢嵟戝抣
	#define MAX_DATACODEWORD	2956			// 僨乕僞僐乕僪儚乕僪嵟戝抣(僶乕僕儑儞40-L)
	#define MAX_CODEBLOCK			153				// 僽儘僢僋僨乕僞僐乕僪儚乕僪悢嵟戝抣(俼俽僐乕僪儚乕僪傪娷傓)
	#define MAX_MODULESIZE			177				// 堦曈儌僕儏乕儖悢嵟戝抣

#else

		// 21:Version=1,最大字符=17(8.5个汉字)
		// 25:Version=2,最大字符=32(16个汉字)
		// 29:Version=3,最大字符=49(24.5个汉字)
		// 33:Version=4,最大字符=78(39个汉字)
		// 37:Version=5,最大字符=106(53个汉字) 
		// 41:Version=6,最大字符=134(67个汉字)
		// 45:Version=7,最大字符=154(77个汉字)
		// 49:Version=8,最大字符=192(96个汉字)

	#define MAX_MODULESIZE			49				
	#define MAX_ALLCODEWORD		(MAX_MODULESIZE*20)	
	#define MAX_DATACODEWORD	(MAX_MODULESIZE*19)	
	#define MAX_CODEBLOCK			153	
		
	//106 1122222222222222222222222222222222233333333333333333333333333333333333333333333333334345444444444444444567
	//134 11222222222222222222222222222222222333333333333333333333333333333333344554445345353455333333333345435535333333334345444444444444444567

#endif

	// 價僢僩儅僢僾昤夋帪儅乕僕儞
	#define QR_MARGIN			4


	/////////////////////////////////////////////////////////////////////////////
	typedef struct tagRS_BLOCKINFO
	{
		int ncRSBlock;			// 俼俽僽儘僢僋悢
		int ncAllCodeWord;		// 僽儘僢僋撪僐乕僪儚乕僪悢
		int ncDataCodeu32;		// 僨乕僞僐乕僪儚乕僪悢(僐乕僪儚乕僪悢 - 俼俽僐乕僪儚乕僪悢)

	} RS_BLOCKINFO, *LPRS_BLOCKINFO;


	/////////////////////////////////////////////////////////////////////////////
	// QR僐乕僪僶乕僕儑儞(宆斣)娭楢忣曬

	typedef struct tagQR_VERSIONINFO
	{
		int nVersionNo;					// 僶乕僕儑儞(宆斣)斣崋(1乣40)
		int ncAllCodeWord;				// 憤僐乕僪儚乕僪悢

										// 埲壓攝楍揧帤偼岆傝掶惓棪(0 = L, 1 = M, 2 = Q, 3 = H) 
		int ncDataCodeu32[4];			// 僨乕僞僐乕僪儚乕僪悢(憤僐乕僪儚乕僪悢 - 俼俽僐乕僪儚乕僪悢)

		int ncAlignPoint;				// 傾儔僀儊儞僩僷僞乕儞嵗昗悢
		int nAlignPoint[6];				// 傾儔僀儊儞僩僷僞乕儞拞怱嵗昗

		RS_BLOCKINFO RS_BlockInfo1[4];	// 俼俽僽儘僢僋忣曬(1)
		RS_BLOCKINFO RS_BlockInfo2[4];	// 俼俽僽儘僢僋忣曬(2)

	} QR_VERSIONINFO, *LPQR_VERSIONINFO;


	/////////////////////////////////////////////////////////////////////////////
	// CQR_Encode 僋儔僗
	
    typedef struct
    {
		int 	nLevel;				// 岆傝掶惓儗儀儖
		int 	nVersion;			// 僶乕僕儑儞(宆斣)
		int 	bAutoExtent;		// 僶乕僕儑儞(宆斣)帺摦奼挘巜掕僼儔僌
		int 	nMaskingNo;			// 儅僗僉儞僌僷僞乕儞斣崋
	
		int 	nSymbleSize;
		u8 		byModuleData[MAX_MODULESIZE][MAX_MODULESIZE]; // [x][y]
		// bit5:婡擻儌僕儏乕儖乮儅僗僉儞僌懳徾奜乯僼儔僌
		// bit4:婡擻儌僕儏乕儖昤夋僨乕僞
		// bit1:僄儞僐乕僪僨乕僞
		// bit0:儅僗僋屻僄儞僐乕僪昤夋僨乕僞
		// 20h偲偺榑棟榓偵傛傝婡擻儌僕儏乕儖敾掕丄11h偲偺榑棟榓偵傛傝昤夋乮嵟廔揑偵偼BOOL抣壔乯
	
		int 	ncDataCodeu32Bit;					// 僨乕僞僐乕僪儚乕僪價僢僩挿
		u8 		byDataCodeu32[MAX_DATACODEWORD];	// 擖椡僨乕僞僄儞僐乕僪僄儕傾
	
		int 	ncDataBlock;
		u8 		byBlockMode[MAX_DATACODEWORD];
		int 	nBlockLength[MAX_DATACODEWORD];
	
		int 	ncAllCodeWord;						// 憤僐乕僪儚乕僪悢(俼俽岆傝掶惓僨乕僞傪娷傓)
		u8 		byAllCodeWord[MAX_ALLCODEWORD];		// 憤僐乕僪儚乕僪嶼弌僄儕傾
		u8 		byRSWork[MAX_CODEBLOCK];			// 俼俽僐乕僪儚乕僪嶼弌儚乕僋
    }QR_DATA;
	//extern QR_DATA 	QR_gData;

// 僨乕僞僄儞僐乕僪娭楢僼傽儞僋僔儑儞

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

// 儌僕儏乕儖攝抲娭楢僼傽儞僋僔儑儞

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
	 * 生成二维码图片文件
	 *
	 * @param pInfo			[in]					需要生成的数据信息数据
	 * @param nLen			[in]					需要生成的数据信息数据长度
	 * @param bHighQuality	[in]					是否生成高质量二维码，YES为高质量效果，NO为普通效果
	 * @param nScale		[in]					二维码图片放大倍数
	 * @param pImgFile		[in]					二维码图片文件名
	 * @param pImgWidth		[out]					二维码图片宽度
	 * @retval {@link RET_OK}  						成功
	 * @retval {@link RET_ERR}						错误
	 */
	extern int Lib_QrCodeImg(IMAGE *rImge,const char* pInfo ,int bHighQuality);



#ifdef __cplusplus
}
#endif

#endif
