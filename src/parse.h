#ifndef BISON_Y_TAB_H
# define BISON_Y_TAB_H

#ifndef YYSTYPE
typedef union {
	mpw_t val;
	char *id;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
# define	STARTTOK	257
# define	LOADFILE	258
# define	LOADFILE_GLOB	259
# define	LOAD_PLUGIN	260
# define	CHANGEDIR	261
# define	PWD	262
# define	LS	263
# define	LS_ARG	264
# define	HELP	265
# define	HELP_ARG	266
# define	NUMBER	267
# define	STRING	268
# define	FUNCID	269
# define	FUNCTION	270
# define	CALL	271
# define	THREEDOTS	272
# define	PARAMETER	273
# define	RETURNTOK	274
# define	BAILOUT	275
# define	EXCEPTION	276
# define	CONTINUE	277
# define	BREAK	278
# define	WHILE	279
# define	UNTIL	280
# define	FOR	281
# define	SUM	282
# define	PROD	283
# define	DO	284
# define	IF	285
# define	THEN	286
# define	ELSE	287
# define	TO	288
# define	BY	289
# define	IN	290
# define	AT	291
# define	MAKEIMAGPARENTH	292
# define	SEPAR	293
# define	NEXTROW	294
# define	EQUALS	295
# define	DEFEQUALS	296
# define	TRANSPOSE	297
# define	ELTELTDIV	298
# define	ELTELTMUL	299
# define	ELTELTEXP	300
# define	ELTELTMOD	301
# define	DOUBLEFACT	302
# define	EQ_CMP	303
# define	NE_CMP	304
# define	CMP_CMP	305
# define	LT_CMP	306
# define	GT_CMP	307
# define	LE_CMP	308
# define	GE_CMP	309
# define	LOGICAL_XOR	310
# define	LOGICAL_OR	311
# define	LOGICAL_AND	312
# define	LOGICAL_NOT	313
# define	LOWER_THEN_ELSE	314
# define	MOD	315
# define	ELTELTBACKDIV	316
# define	UMINUS	317
# define	UPLUS	318


extern YYSTYPE yylval;

#endif /* not BISON_Y_TAB_H */
