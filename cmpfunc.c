#include "cmp.h"

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
    if (node == NULL) return 255;   //do nothing
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
            addr0 = ((struct terminalNode *)node)->value & 0xff;
            addr1 = (((struct terminalNode *)node)->value & 0xff00)>>8;
            fprintf(yyout, "input %d\n", addr0);
            fprintf(yyout, "mov RI AD0\n");
            fprintf(yyout, "input %d\n", addr1);
            fprintf(yyout, "mov RI AD1\n");
            fprintf(yyout, "mov MEM GR%d\n", depth++);
            return depth-1;
            break;
        case ptrSet: // sets address register
            addr0 = ((struct terminalNode *)node)->value & 0xff;
            addr1 = (((struct terminalNode *)node)->value & 0xff00)>>8;
            fprintf(yyout, "input %d\n", addr0);
            fprintf(yyout, "mov RI AD0\n");
            fprintf(yyout, "input %d\n", addr1);
            fprintf(yyout, "mov RI AD1\n");
            return 255;
        case varSet:  // sets address register
            addr0 = ((struct terminalNode *)node)->value & 0xff;
            addr1 = (((struct terminalNode *)node)->value & 0xff00)>>8;
            fprintf(yyout, "input %d\n", addr0);
            fprintf(yyout, "mov RI AD0\n");
            fprintf(yyout, "input %d\n", addr1);
            fprintf(yyout, "mov RI AD1\n");
            return 255;
            break;
        case addByte:   //add
            l = eval(node->l);
            r = eval(node->r);
            fprintf(yyout, "mov GR%d RX\n", l);
            depth--;
            fprintf(yyout, "mov GR%d RY\n", r);
            depth--;
            fprintf(yyout, "compute add\n");
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
            fprintf(yyout, "compute sub\n");
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
        case byteout:
            l = eval(node->l);
            fprintf(yyout, "mov GR%d RO\n", l);
            depth--;
            fprintf(yyout, "output\n");
            return 255;
            break;

        case jmp:
            eval(node->l); 
            fprintf(yyout, "jump\n");
            return 255;
            break;
        case jmplbl:
            l = ((struct terminalNode *)node)->value;
            fprintf(yyout, "input <%d,0>\n", l);
            fprintf(yyout, "mov RI AD0\n");
            fprintf(yyout, "input <%d,1>\n", l);
            fprintf(yyout, "mov RI AD1\n");
            fprintf(yyout, "jump\n");
            return 255;
            break;
        case labelSet:
            fprintf(yyout, "<%d>", ((struct terminalNode *)node)->value);
            return 255;
        default:
            fprintf(yyout, "Node error: %d\n", node->nodeType);
            return 0x255;
    }
}

int newSym(const char *a, struct symtab *table){
    static int currentAddress = VARS_START;
    
    struct symtab *cur = table;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    
    cur->next = malloc(sizeof(struct symtab));
    cur = cur->next;
    cur->name = malloc(sizeof(char)*MAX_VAR_LENGTH);
    strcpy(cur->name, a);
    cur->address = currentAddress++;
    cur->next = NULL;

    return cur->address;
}
int lookupSym(const char *a, struct symtab *table) {
    struct symtab *cur = table;
    if (cur->next == NULL) return -1;

    do {
        cur = cur->next;
        if (!strcmp(cur->name,a)) {
            return cur->address;
        }
        //printf("%s: %d\n", cur->name, cur->address);
    } while (cur->next != NULL);
    return -1;
}


void compile(FILE *source, FILE *output){
    varTable = malloc(sizeof(struct symtab));
    labelTable = malloc(sizeof(struct symtab));
    
    yyrestart(source);
    yyout = output;
    yyparse();
    fclose(source);
}