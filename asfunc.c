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
    switch(node->nodeType) {
        case byteRaw:   // 'b' for byte
            printf("SET @%d TO %d\n", depth++, ((struct terminalNode *)node)->value);
            return depth-1;
            break;
        case addr: //address set
            return eval(node->l);
            break;
            ;
        case varRef: 
            return ((struct terminalNode *)node)->value;
            depth++;
            break;
        case addrRef: //reference to address
            l = eval(node->l);
            depth--;
            //printf("CPY @%d to @%d\n", l, depth++);
            //return eval(node->l);
            printf("READ @@%d TO @%d\n", l, depth++);
            return depth-1;
            break;
        case addByte:   //add
            l = eval(node->l);
            r = eval(node->r);
            depth -= 2;
            printf("ADD @%d @%d @%d\n", l, r, depth++); 
            return depth-1;
            break;
        case subByte:
            l = eval(node->l);
            r = eval(node->r);
            depth -= 2;
            printf("SUB @%d @%d @%d\n", l, r, depth++); 
            return depth-1;
            break;
        case bitAnd:
            l = eval(node->l);
            r = eval(node->r);
            depth -= 2;
            printf("AND @%d @%d @%d\n", l, r, depth++); 
            return depth-1;
            break;
        case bitOr:
            l = eval(node->l);
            r = eval(node->r);
            depth -= 2;
            printf("OR @%d @%d @%d\n", l, r, depth++); 
            return depth-1;
            break;
        case bitXor:
            l = eval(node->l);
            r = eval(node->r);
            depth -= 2;
            printf("XOR @%d @%d @%d\n", l, r, depth++); 
            return depth-1;
            break;
        case bitNot:
            l = eval(node->l);
            depth--;
            printf("NOT @%d @%d\n", l, depth++); 
            return depth-1;
            break;
        case assign:
            printf("WRITE @%d TO @@%d\n", eval(node->r), eval(node->l)); //write
            depth -= 2;
            return -1;
            break;
        default:
            printf("Node error: %d\n", node->nodeType);
            return 0xff;
    }
}
static struct symtab *symbolTable = NULL;
int newSym(char *a){
    static int currentAddress = VARS_START;
    if (!symbolTable) {
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
        strcpy(cur->name, a);
        cur->address = currentAddress++;
        cur->next = NULL;
    }
    return currentAddress-1;
}
int lookupSym(char *a) {
    struct symtab *cur = symbolTable;
    while((strcmp(cur->name,a))) {
        if (cur->next == NULL){
            return -1;
        } else {
            cur = cur->next;
        }
    }
    return cur->address;
}