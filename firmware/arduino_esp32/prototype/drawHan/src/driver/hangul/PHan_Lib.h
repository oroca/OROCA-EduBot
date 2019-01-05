//----------------------------------------------------------------------------
//    프로그램명 : PHan
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 2006.9. 18
//    
//    최종 수정  : 2003.9. 18
//
//    MPU_Type   : 
//
//    파일명     : PHan_Lib.h
//----------------------------------------------------------------------------




//------- 원본이외의 루틴에서 참조시 외부변수로 참조한다
#ifndef  _PHAN_LIB_H
#define  _PHAN_LIB_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Define.h"


#ifdef   PHAN_LIB_LOCAL
#define  EXT_PHAN_LIB_DEF 
#else
#define  EXT_PHAN_LIB_DEF     extern
#endif



#define PHAN_NULL_CODE    0
#define PHAN_HANGUL_CODE  1
#define PHAN_ENG_CODE     2
#define PHAN_SPEC_CODE    3
#define PHAN_END_CODE     4





typedef struct 
{
	U16 HanCode;
	U16 Size_Char;  // 글자 1개의 바이트수(한글:2 영문:1)
	U16 Code_Type;  // 한/영/특수 문자인지 감별 

	char FontBuffer[32];
} PHAN_FONT_OBJ;





EXT_PHAN_LIB_DEF U16  PHan_FontLoad( char *HanCode, PHAN_FONT_OBJ *FontPtr );
EXT_PHAN_LIB_DEF void PHan_DisHanFont(PHAN_FONT_OBJ *FontPtr);
EXT_PHAN_LIB_DEF void PHan_MoveHanFont( char *ImageBuffer, PHAN_FONT_OBJ *FontPtr, int Xpos);
EXT_PHAN_LIB_DEF void PHan_HanFontLoad( char *HanCode, PHAN_FONT_OBJ *FontPtr );
EXT_PHAN_LIB_DEF void PHan_EngFontLoad( char *HanCode, PHAN_FONT_OBJ *FontPtr ) ;
EXT_PHAN_LIB_DEF void PHan_UniFontLoad( char *HanCode, PHAN_FONT_OBJ *FontPtr );

#ifdef __cplusplus
}
#endif

#endif