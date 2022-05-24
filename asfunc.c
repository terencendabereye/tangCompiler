#include "as.h"

struct ast *newNode(enum nodeType type, struct ast* l, struct ast *r){
    struct ast *out = malloc(sizeof(struct ast));
    out->nodeType = type;
    out->l = l;
    out->r = r;
    return out;
}
struct ast *newTerminal(enum nodeType type, int value){
    struct terminalNode *out = malloc(sizeof(struct ast));
    out->value = value;
    out->nodeType = type;
    return (struct ast *)out;
}
int eval(struct ast *node) {
    static int depth = TEMP_VARS_START;
    int l,r;
    int addr0, addr1;
    switch(node->nodeType) {
        case byteRaw:   // 'b' for byte
            fprintf(yyout, "input %d\n", ((struct terminalNode *)node)->value);
            fprintf(yyout, "mov RI GR%d\n", depth++);
            return depth-1;
            break;
        case varRef: 
            addr0 = ((struct terminalNode *)node)->value & 0xff;
            addr1 = (((struct terminalNode *)node)->value & 0xff00)>>8;
            fprintf(yyout, "input %d\n", addr0);
            fprintf(yyout, "mov RI AD0\n");
            fprintf(yyout, "input %d\n", addr1);
            fprintf(yyout, "mov RI AD1\n");
            fprintf(yyout, "mov MEM GR%d\n", depth++);
            return depth-1;
            break;
        case addrRef: //reference to address
            fprintf(yyout, "pending\n");
            return 255;
            break;
        case ptrSet:
            addr0 = ((struct terminalNode *)node)->value & 0xff;
            addr1 = (((struct terminalNode *)node)->value & 0xff00)>>8;
            fprintf(yyout, "input %d\n", addr0);
            fprintf(yyout, "mov RI AD0\n");
            fprintf(yyout, "input %d\n", addr1);
            fprintf(yyout, "mov RI AD1\n");
            return-1;
        case varSet:
            addr0 = ((struct terminalNode *)node)->value & 0xff;
            addr1 = (((struct terminalNode *)node)->value & 0xff00)>>8;
            fprintf(yyout, "input %d\n", addr0);
            fprintf(yyout, "mov RI AD0\n");
            fprintf(yyout, "input %d\n", addr1);
            fprintf(yyout, "mov RI AD1\n");
            return -1;
            break;
        case addByte:   //add
            l = eval(node->l);
            r = eval(node->r);
            fprintf(yyout, "mov GR%d RX\n", l);
            depth--;
            fprintf(yyout, "mov GR%d RY\n", r);
            depth--;
            fprintf(yyout, "add\n");
            fprintf(yyout, "mov RA GR%d\n", depth++);
            return depth-1;
            break;
        case subByte:
            l = eval(node->l);
            r = eval(node->r);
            fprintf(yyout, "mov GR%d RX\n", l);
            depth--;
            fprintf(yyout, "mov GR%d RY\n", r);
            depth--;
            fprintf(yyout, "sub\n");
            fprintf(yyout, "mov RA GR%d\n", depth++);
            return depth-1;
            break;
        case bitAnd:
            // pending
            fprintf(yyout, "pending\n");
            return 255;
            break;
        case bitOr:
            // pending
            fprintf(yyout, "pending\n");
            return 255;
            break;
        case bitXor:
            // pending
            fprintf(yyout, "pending\n");
            return 255;
            break;
        case bitNot:
            // pending
            fprintf(yyout, "pending\n");
            return 255;
            break;
        case assign:
            r = eval(node->r);
            eval(node->l);
            fprintf(yyout, "mov GR%d MEM\n", r);
            depth--;
            return -1;
            break;
        default:
            fprintf(yyout, "Node error: %d\n", node->nodeType);
            return 0x255;
    }
}
struct symtab *symbolTable = NULL;
int newSym(const char *a){
    static int currentAddress = VARS_START;
    if (symbolTable == NULL) {
        symbolTable = malloc(sizeof(struct symtab));
        symbolTable->name = malloc(sizeof(char)*MAX_VAR_LENGTH);
        strcpy(symbolTable->name, a);
        symbolTable->address = currentAddress++;
        symbolTable->next = NULL;
    } else {
        struct symtab *cur = symbolTable;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        
        cur->next = malloc(sizeof(char)*MAX_VAR_LENGTH);
        cur = cur->next;
        cur->name = malloc(sizeof(char)*MAX_VAR_LENGTH);
        strcpy(cur->name, a);
        cur->address = currentAddress++;
        cur->next = NULL;
    }
    return currentAddress-1;
}
int lookupSym(const char *a) {
    struct symtab *cur = symbolTable;
    if (cur == NULL) return -1;

    do {
        if (!strcmp(cur->name,a)) {
            return cur->address;
        }
        cur = cur->next;
    } while (cur!=NULL);
    return -1;
}


int main(int argc, char **argv) {
	FILE *f;
    
    if (argc<2) {
        fflush(stdout);
        fflush(stderr);
        printf("You must specify a source folder\n");
        printf("Usage: tang <source_file> <output_file>\n");
        printf("\ttang <source_file>   outputs to stdout\n");
        return 1;
    }
    if (argc==3) {
        yyout = fopen(argv[2], "w");
        //printf("path: %s\n", argv[2]);
    }
    if ((f=fopen(argv[1], "r"))) {
        yyrestart(f);
        yyparse();
        fclose(f); 
    } else {
        yyerror("File not found\n");
    }
	
	return 0;
}