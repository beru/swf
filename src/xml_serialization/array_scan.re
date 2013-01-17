#include "array_scan.h"

ArrayTokenType array_scan(const char*& p)
{
char* q = 0;
#define	YYCTYPE		char
#define	YYCURSOR	p
#define	YYLIMIT		p
#define	YYMARKER	q
#define	YYFILL(n)	0
/*!re2c
	space = [ \h\t\v\f\r];
	eol = [\r\n];
	eof = "\000";
	value = [^\[\],];
	
	"["			{return ArrayTokenType_LEFT_SQUARE_BRACKET;}
	"]"			{return ArrayTokenType_RIGHT_SQUARE_BRACKET;}
	","			{return ArrayTokenType_COMMA;}
	space+		{return ArrayTokenType_SPACE;}
	eol+		{return ArrayTokenType_EOF;}
	eof			{return ArrayTokenType_EOL;}
	value+		{return ArrayTokenType_VALUE;}
	[^]			{return ArrayTokenType_OTHER;}
*/
}

