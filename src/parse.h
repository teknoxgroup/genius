typedef union {
	mpw_t val;
	char *id;
} YYSTYPE;
#define	STARTTOK	257
#define	LOADFILE	258
#define	LOADFILE_GLOB	259
#define	NUMBER	260
#define	STRING	261
#define	FUNCID	262
#define	FUNCTION	263
#define	CALL	264
#define	RETURNTOK	265
#define	BAILOUT	266
#define	EXCEPTION	267
#define	CONTINUE	268
#define	BREAK	269
#define	WHILE	270
#define	UNTIL	271
#define	FOR	272
#define	DO	273
#define	IF	274
#define	THEN	275
#define	ELSE	276
#define	TO	277
#define	BY	278
#define	IN	279
#define	AT	280
#define	REGION_SEP	281
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
#define	LOWER_THEN_ELSE	295
#define	UMINUS	296
#define	UPLUS	297


extern YYSTYPE yylval;
