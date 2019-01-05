//----------------------------------------------------------------------------
//    프로그램명 : PHan_Lib.c
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 2006.9.18
//    
//    최종 수정  : 2003.9.18
//
//    MPU_Type   : 
//
//    파일명     : PHan_Lib.c
//----------------------------------------------------------------------------

/*


*/


//----- 헤더파일 열기
//
#ifndef   LIB_INCLUDE
#define  PHAN_LIB_LOCAL  
#define  PHANFONT_LOCAL
#define  PHANFONTENG_LOCAL
#include "PHan_Lib.h"
#include "PHanFont.h"
#include "PHanFontEng.h"

#include <stdio.h>
#endif




//----------------------------------------------------------------------------
//
//          TITLE : PHan_CnvCodeWan2Johab
//
//          WORK  : 
//
//----------------------------------------------------------------------------
U16 PHan_CnvCodeWan2Johab(U16 WanCode)
{
	int	index;
	U16	hcode, lcode;

	hcode = (WanCode >> 8) & 0xFF;
	lcode = WanCode & 0x0ff;

	index = (hcode - 0x0B0) * 94 + (lcode - 0x0A1);

	return wWanToJohabTable[index];
}





//----------------------------------------------------------------------------
//
//          TITLE : PHan_FontLoad
//
//          WORK  : 
//
//----------------------------------------------------------------------------
/*----------------------------------------------------------------------*/
/*	한글 일반 폰트(24x24)를 bTemp1Font[72]에 LOAD한다.					*/
/*----------------------------------------------------------------------*/
U16 PHan_FontLoad( char *HanCode, PHAN_FONT_OBJ *FontPtr )   /* 한글 일반 폰트 생성 */
{

	//static declaration 은 속도를 높이기 위한것임.
	static U16 uChosung, uJoongsung, uJongsung, uChoType, uJooType,uJonType;

	// 버퍼 초기화
	memset(FontPtr->FontBuffer, 0x00, 32);


	FontPtr->Code_Type = PHAN_NULL_CODE;	
	// 한글코드인지 감별 
	// 
	if( !HanCode[0] || HanCode[0] == 0x0A )   // 문자열 마지막
	{
		FontPtr->Code_Type = PHAN_END_CODE;
		FontPtr->Size_Char = 1;
		return PHAN_END_CODE;
	}
	else if( HanCode[0] & 0x80 )              // 한글 코드인경우 
	{
		U16 utf16;
		U32 char_code;

		char_code = (HanCode[0]<<16) | (HanCode[1]<<8) | (HanCode[2]<<0);

		//utf16 = (HanCode[0] & 0x0f) << 12 | (HanCode[1] & 0x3f) << 6 | HanCode[2] & 0x3f;

		if (char_code >= 0xEAB080 && char_code <= 0xED9FB0)
		//if (utf16 >= 0xAC00 && utf16 <= 0xD7FF)
		{
			FontPtr->Code_Type = PHAN_HANGUL_CODE;
			FontPtr->Size_Char = 3;
			PHan_UniFontLoad( HanCode, FontPtr );
		}
		else
		{
			FontPtr->Code_Type = PHAN_HANGUL_CODE;
			FontPtr->Size_Char = 2;
			PHan_HanFontLoad( HanCode, FontPtr );
		}
		return PHAN_HANGUL_CODE;
	}
	else                                      // 영문 코드 
	{
		FontPtr->Code_Type = PHAN_ENG_CODE;
		FontPtr->Size_Char = 1;
		PHan_EngFontLoad( HanCode, FontPtr );
		return PHAN_ENG_CODE;
	}


	return FontPtr->Code_Type;

}





//----------------------------------------------------------------------------
//
//          TITLE : PHan_FontLoad
//
//          WORK  : 
//
//----------------------------------------------------------------------------
void PHan_HanFontLoad( char *HanCode, PHAN_FONT_OBJ *FontPtr )   /* 한글 일반 폰트 생성 */
{
	U16 i;
	U16 wHanCode;	
	//static declaration 은 속도를 높이기 위한것임.
	static U16 uChosung, uJoongsung, uJongsung, uChoType, uJooType,uJonType;

	wHanCode = (U16)HanCode[0]<<8  | (U16)HanCode[1] & 0x00FF;
	
	//printf("%x %x %x\n", HanCode[0], HanCode[1], HanCode[2] );

	wHanCode = PHan_CnvCodeWan2Johab(wHanCode);
	//printf("a %x \n", wHanCode );

	//seperate phoneme code
	uChosung   = (wHanCode>>10)&0x001F;//Chosung code
	uJoongsung = (wHanCode>>5) &0x001F;//Joongsung code
	uJongsung  = wHanCode & 0x001F;    //Jongsung code

	//make font index
	uChosung   = ChoIdxTbl[uChosung];    //Chosung index
	uJoongsung = JooIdxTbl[uJoongsung];  //Joongsung index
	uJongsung  = JonIdxTbl[uJongsung];   //Jongsung index

	//decide a character type (몇번째 벌을 사용할지 결정)
	uChoType = uJongsung ? ChoTypeCaseJongYes[uJoongsung]:ChoTypeCaseJongNo [uJoongsung];
	  //'ㄱ'(1) 이나 'ㅋ'(16) 인경우는
	uJooType = ((uChosung == 0 || uChosung == 1 ||uChosung == 16 ) ? 0: 1) + (uJongsung ? 2: 0);
	uJonType = JonType[uJoongsung];

	for(i = 0; i<32; i++)
	{
		FontPtr->FontBuffer[i]  = K_font[uChoType*20+uChosung][i];
		FontPtr->FontBuffer[i] |= K_font[160 + uJooType*22+uJoongsung][i];
	}

	//combine Jongsung
	if(uJongsung)
	{
		for(i = 0; i < 32; i++) 	FontPtr->FontBuffer[i] |= K_font[248 + uJonType*28+uJongsung][i];
	}
}




//----------------------------------------------------------------------------
//
//          TITLE : PHan_UniFontLoad
//
//          WORK  : 
//
//----------------------------------------------------------------------------
void PHan_UniFontLoad( char *HanCode, PHAN_FONT_OBJ *FontPtr )   /* 한글 일반 폰트 생성 */
{
	U16 i;	
	U16 utf16;

	//static declaration 은 속도를 높이기 위한것임.
	static U16 uChosung, uJoongsung, uJongsung, uChoType, uJooType,uJonType;

	
	

	utf16 = (HanCode[0] & 0x0f) << 12 | (HanCode[1] & 0x3f) << 6 | HanCode[2] & 0x3f;
	

	//seperate phoneme code
	utf16 -= 0xac00;
	uJongsung  = utf16 % 28;
	utf16 /= 28;
	uJoongsung = utf16 % 21;
	uChosung   = utf16 / 21;


	//make font index
	uChosung   = UniChoIdxTbl[uChosung];    //Chosung index
	uJoongsung = UniJooIdxTbl[uJoongsung];  //Joongsung index
	uJongsung  = UniJonIdxTbl[uJongsung];   //Jongsung index


	//decide a character type (몇번째 벌을 사용할지 결정)
	uChoType = uJongsung ? ChoTypeCaseJongYes[uJoongsung]:ChoTypeCaseJongNo [uJoongsung];
	  //'ㄱ'(1) 이나 'ㅋ'(16) 인경우는
	uJooType = ((uChosung == 0 || uChosung == 1 ||uChosung == 16 ) ? 0: 1) + (uJongsung ? 2: 0);
	uJonType = JonType[uJoongsung];

	for(i = 0; i<32; i++)
	{
		FontPtr->FontBuffer[i]  = K_font[uChoType*20+uChosung][i];
		FontPtr->FontBuffer[i] |= K_font[160 + uJooType*22+uJoongsung][i];
	}

	//combine Jongsung
	if(uJongsung)
	{
		for(i = 0; i < 32; i++) 	FontPtr->FontBuffer[i] |= K_font[248 + uJonType*28+uJongsung][i];
	}
}






//----------------------------------------------------------------------------
//
//          TITLE : PHan_EngFontLoad
//
//          WORK  : 
//
//----------------------------------------------------------------------------
void PHan_EngFontLoad( char *HanCode, PHAN_FONT_OBJ *FontPtr ) 
{
	U16 i;
	char EngCode;

	EngCode = *HanCode;

	EngCode -= 0x20;  // FONT는 스페이스 부터 시작한다.

	for ( i = 0 ; i < 16 ; i++ )
	{
		 FontPtr->FontBuffer[ i ] = wEngFon[EngCode][i];		
	}
}



/*----------------------------------------------------------------------*/
/*	bTemp1Font[72]에 조합한 한글 일반 폰트를							*/
/*	Line Image Buffer로 전송한다.										*/
/*----------------------------------------------------------------------*/
void PHan_MoveHanFont( char *ImageBuffer, PHAN_FONT_OBJ *FontPtr, int Xpos)
{
	int	i, j;


	for ( j = 0 ; j < 2 ; j++ )		// 16 x 16 (2 Bytes)
	{
		ImageBuffer[ Xpos*2 + j ] = FontPtr->FontBuffer[i*2 +j];
	}
}







void PHan_DisHanFont(PHAN_FONT_OBJ *FontPtr)
{
	U16	i, j, Loop;
	U16 FontSize = FontPtr->Size_Char;


	for ( i = 0 ; i < 16 ; i++ )		// 16 Lines per Font/Char
	{
		for ( j = 0 ; j < FontSize ; j++ )		// 16 x 16 (2 Bytes)
		{
			for( Loop=0; Loop<8; Loop++ )
			{
				if( FontPtr->FontBuffer[i*FontSize +j] & (0x80>>Loop)) printf("*");
				else                                         printf(" ");
				
			}
		}
		printf("   - \n");
		//printf("%x\n",FontPtr->FontBuffer[i]);
	}
}