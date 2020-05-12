
typedef union {
	int	number;
	char	*string;
} YYSTYPE;
extern YYSTYPE yylval;
# define NUMBER 257
# define STRING 258
# define QUOTE_STRING 259
# define EVAR 260
# define COMMAND 261
# define COMMA 262
# define EQUAL 263
# define SCREEN_TITLE 264
# define SCREEN_ROWS 265
# define SCREEN_COLS 266
# define SCREEN_BORDER 267
# define SCREEN_POS 268
# define SCREEN_HELP 269
# define EXIT_LAST_FIELD 270
# define FIELD_NAME 271
# define FIELD_LABEL 272
# define FIELD_ROW 273
# define FIELD_COL 274
# define FIELD_MASK 275
# define FIELD_RANGE 276
# define FIELD_LENGTH 277
# define FIELD_MIN 278
# define FIELD_TYPE 279
# define FIELD_ADJUST 280
# define FIELD_MUSTENTER 281
# define FIELD_TERMINATOR 282
# define FIELD_PROMPT 283
# define FIELD_DEFAULTS 284
# define FIELD_NOINPUT 285
# define FIELD_INCLUDE_MASK 286
