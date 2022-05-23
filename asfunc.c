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
            printf("input %d\n", ((struct terminalNode *)node)->value);
            printf("mov RI GR%d\n", depth++);
            return depth-1;
            break;
        case varRef: 
            addr0 = ((struct terminalNode *)node)->value & 0xff;
            addr1 = (((struct terminalNode *)node)->value & 0xff00)>>8;
            printf("input %d\n", addr0);
            printf("mov RI AD0\n");
            printf("input %d\n", addr1);
            printf("mov RI AD1\n");
            printf("mov MEM GR%d\n", depth++);
            return depth-1;
            break;
        case addrRef: //reference to address
            printf("pending\n");
            return 255;
            break;
        case ptrSet:
            addr0 = ((struct terminalNode *)node)->value & 0xff;
            addr1 = (((struct terminalNode *)node)->value & 0xff00)>>8;
            printf("input %d\n", addr0);
            printf("mov RI AD0\n");
            printf("input %d\n", addr1);
            printf("mov RI AD1\n");
            return-1;
        case varSet:
            addr0 = ((struct terminalNode *)node)->value & 0xff;
            addr1 = (((struct terminalNode *)node)->value & 0xff00)>>8;
            printf("input %d\n", addr0);
            printf("mov RI AD0\n");
            printf("input %d\n", addr1);
            printf("mov RI AD1\n");
            return -1;
            break;
        case addByte:   //add
            l = eval(node->l);
            r = eval(node->r);
            printf("mov GR%d RX\n", l);
            depth--;
            printf("mov GR%d RY\n", r);
            depth--;
            printf("add\n");
            printf("mov RA GR%d\n", depth++);
            return depth-1;
            break;
        case subByte:
            l = eval(node->l);
            r = eval(node->r);
            printf("mov GR%d RX\n", l);
            depth--;
            printf("mov GR%d RY\n", r);
            depth--;
            printf("sub\n");
            printf("mov RA GR%d\n", depth++);
            return depth-1;
            break;
        case bitAnd:
            // pending
            printf("pending\n");
            return 255;
            break;
        case bitOr:
            // pending
            printf("pending\n");
            return 255;
            break;
        case bitXor:
            // pending
            printf("pending\n");
            return 255;
            break;
        case bitNot:
            // pending
            printf("pending\n");
            return 255;
            break;
        case assign:
            r = eval(node->r);
            eval(node->l);
            printf("mov GR%d MEM\n", r);
            depth--;
            return -1;
            break;
        default:
            printf("Node error: %d\n", node->nodeType);
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
