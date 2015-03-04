{RUNE_LIT} {
	if(yyleng == 4){/*two single quotes and a backslash and a LETTER*/
		switch(yytext[2]){
		case 'a':
		yylval.runeconst = '\a';
		break;
		...
		default:
		yyerror("unknown escape sequence\n");
		yyterminate();
		}
	}else
		yylval.runeconst = yytext[1];
    lastToken = tRUNECONST;
	return tRUNECONST;