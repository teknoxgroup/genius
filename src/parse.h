typedef union {
	mpw_t val;
	char *id;
} YYSTYPE;
#define	STARTTOK	258
#define	LOADFILE	259
#define	LOADFILE_GLOB	260
#define	NUMBER	261
#define	STRING	262
#define	FUNCID	263
#define	FUNCTION	264
#define	DEFINE	265
#define	CALL	266
#define	RETURNTOK	267
#define	BAILOUT	268
#define	EXCEPTION	269
#define	CONTINUE	270
#define	BREAK	271
#define	WHILE	272
#define	UNTIL	273
#define	FOR	274
#define	DO	275
#define	IF	276
#define	THEN	277
#define	ELSE	278
#define	TO	279
#define	BY	280
#define	IN	281
#define	AT	282
#define	REGION_SEP	283
#define	SEPAR	284
#define	EQUALS	285
#define	EQ_CMP	286
#define	NE_CMP	287
#define	CMP_CMP	288
#define	LT_CMP	289
#define	GT_CMP	290
#define	LE_CMP	291
#define	GE_CMP	292
#define	LOGICAL_XOR	293
#define	LOGICAL_OR	294
#define	LOGICAL_AND	295
#define	LOGICAL_NOT	296
#define	LOWER_THEN_ELSE	297
#define	UMINUS	298


extern YYSTYPE yylval;
