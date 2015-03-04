"\"" {						/*INTERPRETED STRING*/
	char *r;
	if(r = buildInterpretedString()){
		yylval.stringconst = strdup(r);
		free(r);
		lastToken = tINTERPRETEDSTRING;
		return tINTERPRETEDSTRING;
	}else{
		printf("EOF in string\n");
		yyterminate();
	}
}