")" {lastToken = ')'; return ')';}
"]" {lastToken = ']'; return ']';}
"}" {lastToken = '}'; return '}';}
...
\n {
	lineno++;
	int token = checkLastToken(); 
	if (token == ';') {
		return token;
	}
}