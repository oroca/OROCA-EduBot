//----------------------------------------------------------------------------
//
//    프로그램명 :
//
//    만든이     : 
//
//    날  짜     : 2006.5.24
//
//    파일명     : Define.h
//
//----------------------------------------------------------------------------

#ifndef _DEFINE_H_
#define _DEFINE_H_


typedef  unsigned int        U32;   
typedef  short    int        U16; 
typedef  signed   int        S32;
typedef  short    int        S16;
typedef  unsigned char       U8;
typedef  signed   char       S8;
typedef           float      FLOAT;


#define SET_BIT(BitPort, BitNum)      BitPort |= (1<<(BitNum))
#define CLR_BIT(BitPort, BitNum)      BitPort &= (~(1<<(BitNum)))     
#define TGL_BIT(val,bit)	          (val)   ^= (1<<bit)

#define IS_CLR_BIT(BitPort, BitNum)   (!(BitPort & (1<<BitNum)))
#define IS_SET_BIT(BitPort, BitNum)   (BitPort & (1<<BitNum))



// 내가 선언한가랑 약간 다른다. (1<<x) 형식을 사용하지 않는다.
//
#define SET(BitPort, BitNum)           (BitPort) |=  (BitNum)
#define CLR(BitPort, BitNum)           (BitPort) &= (~(BitNum))
#define IS_CLR(BitPort, BitNum)        (!((BitPort) & (BitNum)))
#define IS_SET(BitPort, BitNum)        ((BitPort) & (BitNum))


// 리얼 시스 때문에 추가했다.
//
//#define 	SET(val,bit)	(val) |= (bit)
#define 	CLEAR(val,bit)	(val) &= (~(bit))
#define 	TOGGLE(val,bit)	(val) ^= (bit)



//----- Q Math 관련
//
#define BIT17RES       65536.
#define BIT16RES       32768.
#define BIT7RES        127.
#define BIT8RES        256.
#define QMATH_MAX   BIT17RES

#define Q_TO_DEC(x)  (S32)(x*QMATH_MAX)

typedef union
{
	S16 BitS16[2];
	S32 BitS32;

	U16 BitU16[2];
	U32 BitU32;

} Q_MATH_OBJ;



typedef union
{
	S16 BitS16[2];
	S32 BitS32;

	U16 BitU16[2];
	U32 BitU32;
	
} WORD32_OBJ;


typedef union               // 16비트 변수를 1바이트씩 나누어 저장
{
	U8  Bit8[2];
	U16 Bit16;
	U8  BitU8[2];
	U16 BitU16;
	
	S8  BitS8[2];
	S16 BitS16;	
} WORD_OBJ;






#endif
