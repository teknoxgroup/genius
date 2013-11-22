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
#define	WHILE	265
#define	UNTIL	266
#define	DO	267
#define	IF	268
#define	THEN	269
#define	ELSE	270
#define	NEG	271
#define	AT	272
#define	SEPAR	273
#define	EQUALS	274
#define	EQ_CMP	275
#define	NE_CMP	276
#define	CMP_CMP	277
#define	LT_CMP	278
#define	GT_CMP	279
#define	LE_CMP	280
#define	GE_CMP	281
#define	LOGICAL_XOR	282
#define	LOGICAL_OR	283
#define	LOGICAL_AND	284
#define	LOGICAL_NOT	285
#define	LOWER_THEN_ELSE	286
#define	UMINUS	287


extern YYSTYPE yylval;
