typedef union {
	mpw_t val;
	char *id;
} YYSTYPE;
#define	STARTTOK	258
#define	NUMBER	259
#define	STRING	260
#define	FUNCID	261
#define	DEFINE	262
#define	CALL	263
#define	RETURNTOK	264
#define	BAILOUT	265
#define	EXCEPTION	266
#define	WHILE	267
#define	UNTIL	268
#define	DO	269
#define	IF	270
#define	THEN	271
#define	ELSE	272
#define	AT	273
#define	SEPAR	274
#define	EQUALS	275
#define	EQ_CMP	276
#define	NE_CMP	277
#define	CMP_CMP	278
#define	LT_CMP	279
#define	GT_CMP	280
#define	LE_CMP	281
#define	GE_CMP	282
#define	LOGICAL_XOR	283
#define	LOGICAL_OR	284
#define	LOGICAL_AND	285
#define	LOGICAL_NOT	286
#define	LOWER_THEN_ELSE	287
#define	UMINUS	288


extern YYSTYPE yylval;
