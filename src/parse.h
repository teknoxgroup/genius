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
#define	DEFINE	264
#define	CALL	265
#define	RETURNTOK	266
#define	BAILOUT	267
#define	EXCEPTION	268
#define	CONTINUE	269
#define	BREAK	270
#define	WHILE	271
#define	UNTIL	272
#define	DO	273
#define	IF	274
#define	THEN	275
#define	ELSE	276
#define	AT	277
#define	REGION_SEP	278
#define	SEPAR	279
#define	EQUALS	280
#define	EQ_CMP	281
#define	NE_CMP	282
#define	CMP_CMP	283
#define	LT_CMP	284
#define	GT_CMP	285
#define	LE_CMP	286
#define	GE_CMP	287
#define	LOGICAL_XOR	288
#define	LOGICAL_OR	289
#define	LOGICAL_AND	290
#define	LOGICAL_NOT	291
#define	LOWER_THEN_ELSE	292
#define	UMINUS	293


extern YYSTYPE yylval;
