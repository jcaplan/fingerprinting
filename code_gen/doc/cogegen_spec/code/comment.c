/*start condition*/
%x comment  

/* regex + action rules come after the first %% */
%%

"/*"   					{BEGIN(comment);}
<comment>[^*\n]*		/*ignore not a star*/ 	
<comment>"*"[^*/\n]* 	/*ignore * not followed by '/'*/
<comment>\n 			lineno++;
<comment>"*/" 			BEGIN(INITIAL);
<comment><<EOF>> 		{printf("comment must end before eof\n");yyterminate();}