typedef union {
	mpw_t val;
	func_t *func;
	char *id;
} YYSTYPE;
#define	POSNUM	258
#define	NEGNUM	259
#define	FUNCID0	260
#define	FUNCID1	261
#define	FUNCID2	262
#define	FUNCID3	263
#define	FUNCID4	264
#define	FUNCID5	265
#define	FUNCID6	266
#define	FUNCID7	267
#define	FUNCID8	268
#define	FUNCDEF	269
#define	INLINE_INFIX	270
#define	INLINE_PREFIX	271
#define	INLINE_POSTFIX	272
#define	BEGINBLOCK	273
#define	ENDBLOCK	274
#define	WHILE_CONS	275
#define	IF_CONS	276
#define	IFELSE_CONS	277
#define	INFIX_EXPR	278
#define	POSTFIX_EXPR	279
#define	PREFIX_EXPR	280
#define	NEG	281
#define	SEPAR	282
#define	EQUALS	283
#define	EQ_CMP	284
#define	NE_CMP	285
#define	CMP_CMP	286
#define	LT_CMP	287
#define	GT_CMP	288
#define	LE_CMP	289
#define	GE_CMP	290
#define	LOGICAL_XOR	291
#define	LOGICAL_OR	292
#define	LOGICAL_AND	293
#define	LOGICAL_NOT	294
#define	UMINUS	295


extern YYSTYPE yylval;
