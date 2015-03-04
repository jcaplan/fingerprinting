//Node Type 
typedef struct ASTNODE {
	enum {PROGRAM_NODE, EXPRESSION_NODE, FUNCTION_NODE, DECLARE_NODE, ARRAY_NODE, IDENTIFIER_NODE, CASE_NODE, TERM_NODE, STATEMENT_NODE, TYPEDEF_NODE, TYPE_NODE, STRUCT_NODE} kind;
	union {
		program_t *pgm;
		expression_t *exp;
		function_t *function;
		declare_t *declaration;
		identifier_t *id;
		term_t *term;
		statement_t *stm;
		typedef_t *type_declaration;
		type_t *type;
		struct_t *structure;
		case_t *case_node;
	}data;
}ASTNODE;
//Node wrapper for struct type
ASTNODE* makeSTRUCTDEFnode(NODELIST* fieldslist){
	ASTNODE *new_node = calloc(1, sizeof(ASTNODE));
	new_node->kind = STRUCT_NODE;
	new_node->data.structure = makeStruct(fieldslist->list.declarations);
	return new_node;
}

//Actual struct builder
struct_t* makeStruct(struct DECLARE_LIST *elements){
	struct_t *structure = calloc(1, sizeof(struct_t));
	structure->internals = elements;
	return structure;
}