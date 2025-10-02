//Mușuroi David-Gabriel - 311CC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Stiva
typedef struct stack{
    struct page *crtPage;
    struct stack *next;
}*Stack;

typedef struct page{
    int ID;
    char url[50];
    char *description;
}Page;

typedef struct tab{
    int id;
    struct page *currentPage;
    struct stack *backwardStack;
    struct stack *forwardStack;
}*Tab;

//Salvez paginile sub forma de lista
typedef struct pages{
    struct page *crt;
    struct pages *next;
}*Pages;

//Lista dublu inlantuita circulara cu santinela
typedef struct tabsList{
    struct tab *crt;
    struct tabsList *next;
    struct tabsList *prev;
}*list;

typedef struct browser{
    struct tab *current;
    struct tabsList *list;
}*Browser;

void free_stack(Stack s){
    while(s != NULL){
        if(s->crtPage->ID == 0){
            free(s->crtPage->description);
            free(s->crtPage);
        }
        Stack p = s;
        s = s->next;
        free(p);
    }
}

void free_pages(Pages p){
    while(p != NULL){
        Pages q = p;
        p = p->next;
        free(q->crt->description);
        free(q->crt);
        free(q);
    }
}

void free_browser(Browser b){
    list loop_tabs = b->list->next, elim_tabs;
    Stack s, p;
    int okay;
    //Cat timp inca exista tab-uri
    while(loop_tabs != b->list){
        okay = 0;
        elim_tabs = loop_tabs;
        loop_tabs = loop_tabs->next;
        s = elim_tabs->crt->backwardStack;
        p = elim_tabs->crt->forwardStack;
        //Daca pagina implicita se afla pe pagina curenta
        if(elim_tabs->crt->currentPage->ID == 0){
            if(elim_tabs->crt->backwardStack != NULL){
                free_stack(elim_tabs->crt->backwardStack);
            }
            if(elim_tabs->crt->forwardStack != NULL){
                free_stack(elim_tabs->crt->forwardStack);
            }
            okay = 1;
            free(elim_tabs->crt->currentPage->description);
            free(elim_tabs->crt->currentPage);
            free(elim_tabs->crt);
            free(elim_tabs);
        }
        //Daca pagina implicita nu s-a gasit pe pagina curenta
        if(okay == 0){
            if(s != NULL){
                free_stack(s);
            }
            if(p != NULL){
                free_stack(p);
            }
            free(elim_tabs->crt);
            free(elim_tabs);
        }
    }
    free(b->list);
    free(b);
}

//Initializez paginile
Pages citire(Pages p, FILE *file_in){
    Pages new_page = malloc(sizeof(struct pages));
    char tmp[1001];
    new_page->crt = malloc(sizeof(struct page));
    fscanf(file_in, "%d", &new_page->crt->ID);
    fgetc(file_in);
    fgets(new_page->crt->url, 50, file_in);
    //Pun terminatorul in locul newline-ului la url si description
    new_page->crt->url[strlen(new_page->crt->url) - 1] = '\0';
    fgets(tmp, 1001, file_in);
    tmp[strlen(tmp) - 1] = '\0';
    new_page->crt->description = malloc(strlen(tmp) + 1);
    strcpy(new_page->crt->description, tmp);
    //Actualizez lista paginilor
    p->next = new_page;
    return new_page;
}

//Initializez browserul
Browser init_browser(){
    //Initializez primul tab
    char tmp[1001];
    Browser b = malloc(sizeof(struct browser));
    b->current = malloc(sizeof(struct tab));
    b->current->id = 0;
    b->current->currentPage = malloc(sizeof(struct page));
    b->current->currentPage->ID = 0;
    strcpy(b->current->currentPage->url, "https://acs.pub.ro/");
    strcpy(tmp, "Computer Science");
    b->current->currentPage->description = malloc(strlen(tmp) + 1);
    strcpy(b->current->currentPage->description, tmp);
    //Construiesc lista
    b->list = malloc(sizeof(struct tabsList));
    b->list->crt = NULL;
    list new = malloc(sizeof(struct tabsList));
    //Leg santinela de tab-ul cu id-ul 0
    b->list->next = new;
    b->list->prev = new;
    new->next = b->list;
    new->prev = b->list;
    //Actualizez lista de tab-uri
    b->current->backwardStack = b->current->forwardStack = NULL;
    new->crt = b->current;
    return b;
}

void NEW_TAB(Browser b, int id_local){
    //Caut ultimul tab
    list c = b->list->next;
    while(c->next != b->list){
        c = c->next;
    }
    char tmp[1001];
    //Adaug un nou tab la lista
    list new = malloc(sizeof(struct tabsList));
    b->list->prev = new;
    c->next = new;
    new->prev = c;
    new->next = b->list;
    //Creez un nou tab cu pagina implicita
    new->crt = malloc(sizeof(struct tab));
    new->crt->currentPage = malloc(sizeof(struct page));
    new->crt->id = id_local;
    new->crt->currentPage->ID = 0;
    strcpy(new->crt->currentPage->url, "https://acs.pub.ro/");
    strcpy(tmp, "Computer Science");
    new->crt->currentPage->description = malloc(strlen(tmp) + 1);
    strcpy(new->crt->currentPage->description, tmp);
    new->crt->backwardStack = new->crt->forwardStack = NULL;
    //Actualizez tab-ul curent
    b->current = new->crt;
}

void CLOSE(Browser b, FILE *file_out){
    if(b->current->id == 0){
        fprintf(file_out, "403 Forbidden\n");
    }
    else{
        //Iterez prin lista de tab-uri pana ajung la tabul curent
        list c = b->list->next, d = b->list->next;
        while(d->crt->id != b->current->id){
            d = d->next;
        }
        while(c->next != d){
            c = c->next;
        }
        //Elimin tab-ul curent, il reinitializez si restabilesc legaturile
        b->current = c->crt;
        c->next = d->next;
        d->next->prev = c;
        //Eliberez tab-ul eliminat
        if(d->crt->currentPage->ID == 0){
            free(d->crt->currentPage->description);
            free(d->crt->currentPage);
            free_stack(d->crt->backwardStack);
            free_stack(d->crt->forwardStack);
            free(d->crt);
            free(d);
        }
        else{
            free_stack(d->crt->backwardStack);
            free_stack(d->crt->forwardStack);
            free(d->crt);
            free(d);
        }
    }
}

//Caut tab-ul cu id-ul Id
void OPEN(Browser b, int Id, FILE *file_out){
    int found = 0;
    list c = b->list->next;
    //Iterez prin lista de tab-uri
    while(c->next != b->list && found == 0){
        if(c->crt->id == Id){
            found = 1;
            break;
        }
            c = c->next;
    }
    if(c->crt->id == Id){
        found = 1;
    }
    //Actualizez tab-ul curent sau afisez eroarea
    if(found == 0){
        fprintf(file_out, "403 Forbidden\n");
    }
    else{
        b->current = c->crt;;
    }
}

void NEXT(Browser b){
    list c = b->list->next;
    //Caut tab-ul curent
    while(c->crt != b->current){
        c = c->next;
    }
    //Trec peste el
    c = c->next;
    if(c == b->list){
        c = c->next;
    }
    //Actualizez tab-ul curent
    b->current = c->crt;
}

void PREV(Browser b){
    list c = b->list->next;
    //Caut tab-ul curent
    while(c->crt != b->current){
        c = c->next;
    }
    //Ma duc inaintea lui
    c = c->prev;
    if(c == b->list){
        c = c->prev;
    }
    //Actualizez tab-ul curent
    b->current = c->crt;
}

void PAGE(Browser b, int Id, Pages p, FILE *file_out){
    free_stack(b->current->forwardStack);
    b->current->forwardStack = NULL;
    //Daca nu exista pagini
    if(p == NULL){
        fprintf(file_out, "403 Forbidden\n");
        return;
    }
    //Id-ul este pe pagina implicita
    if(p->crt->ID == Id){
        Stack new_stack = b->current->backwardStack;
        //Construiesc head-ul stivei
        if(new_stack == NULL){
            b->current->backwardStack = malloc(sizeof(struct stack));
            b->current->backwardStack->crtPage = b->current->currentPage;
            b->current->backwardStack->next = NULL;
            b->current->currentPage = p->crt;
        }
        else{
            //Iterez pana ajung la ultimul element al stivei
            while(new_stack->next != NULL){
                new_stack = new_stack->next;
            }
            //Actualizez stiva
            new_stack->next = malloc(sizeof(struct stack));
            new_stack->next->crtPage = b->current->currentPage;
            new_stack->next->next = NULL;
            b->current->currentPage = p->crt;
        }
    }
    //Id-ul este pe o alta pagina
    else{
        int found = 0;
        //Am decat pagina implicita
        if(p->next == NULL){
            fprintf(file_out, "403 Forbidden\n");
            return;
        }
        //Caut pagina cu id-ul Id
        else{
            Pages new_page = p->next;
            while(new_page != NULL){
                if(new_page->crt->ID == Id){
                    found = 1;
                    break;
                }
                else{
                    new_page = new_page->next;
                }
            }
            //Afisez eroare
            if(found == 0){
                fprintf(file_out, "403 Forbidden\n");
                return;
            }
            Stack new_stack = b->current->backwardStack;
            //Construiesc head-ul stivei
            if(new_stack == NULL){
                b->current->backwardStack = malloc(sizeof(struct stack));
                b->current->backwardStack->crtPage = b->current->currentPage;
                b->current->backwardStack->next = NULL;
                b->current->currentPage = new_page->crt;
            }
            else{
                //Iterez pana ajung la ultimul element al stivei
                while(new_stack->next != NULL){
                    new_stack = new_stack->next;
                }
                //Actualizez stiva
                new_stack->next = malloc(sizeof(struct stack));
                new_stack->next->crtPage = b->current->currentPage;
                new_stack->next->next = NULL;
                b->current->currentPage = new_page->crt;
            }
        }
    }
}

void BACKWARD(Browser b, FILE *file_out){
    Stack s, p, q, r;
    s = b->current->backwardStack;
    r = b->current->backwardStack;
    if(s == NULL){
        fprintf(file_out, "403 Forbidden\n");
        return;
    }
    //Iterez pana la ultima pagina din backwardStack
    while(s->next != NULL){
        s = s->next;
    }
    //Iterez pana la ultima pagina din forwardStack
    p = b->current->forwardStack;
    q = malloc(sizeof(struct stack));
    if(p == NULL){
        b->current->forwardStack = q;
    }
    else{
        while(p->next != NULL){
            p = p->next;
        }
        p->next = q;
    }
    q->next = NULL;
    //Introduc in forwardStack pagina curenta
    q->crtPage = b->current->currentPage;
    //Setez ultima pagina din backwardStack ca si pagina curenta
    b->current->currentPage = s->crtPage;
    //Eliberez ultima pagina din backwardStack
    if(r->next == NULL){
        free(r);
        b->current->backwardStack = NULL;
    }
    else{
        while(r->next != s){
            r = r->next;
        }
        r->next = NULL;
        free(s);
    }
}

void FORWARD(Browser b, FILE *file_out){
    Stack s, p, q, r;
    s = b->current->backwardStack;
    p = b->current->forwardStack;
    q = b->current->backwardStack;
    if(p == NULL){
        fprintf(file_out, "403 Forbidden\n");
    }
    else{
    //Iterez pana la ultima pagina din backwardStack
        if(s != NULL){
            while(s->next != NULL){
                s = s->next;
            }
            q = malloc(sizeof(struct stack));
            s->next = q;
        }
        else if(s == NULL){
            q = malloc(sizeof(struct stack));
            b->current->backwardStack = q;
        }
        q->next = NULL;
        //Introduc in backwardStack pagina curenta
        q->crtPage = b->current->currentPage;
        //Setez ultima pagina din forwardStack ca si pagina curenta
        while(p->next != NULL){
            p = p->next;
        }
        b->current->currentPage = p->crtPage;
        //Scot ultima pagina din forwardStack
        r = b->current->forwardStack;
        if(r->next == NULL){
            free(r);
            b->current->forwardStack = NULL;
        }
        else{
            while(r->next != p){
                r = r->next;
            }
            r->next = NULL;
            free(p);
        }   
    }
}

void PRINT(Browser b, FILE *file_out){
    //Iterez prin lista de tab-uri pana ajung la tab-ul curent
    list new_tab = b->list->next;
    while(new_tab->crt != b->current){
        new_tab = new_tab->next;
    }
    fprintf(file_out, "%d", new_tab->crt->id);
    new_tab = new_tab->next;
    //Afisez toate tab-urile pana la tab-ul curent
    while(new_tab->crt != b->current){
        //Trec peste santinela
        if(new_tab->next == b->list->next){
            new_tab = new_tab->next;
        }
        else{
            fprintf(file_out, " %d", new_tab->crt->id);
            new_tab = new_tab->next;
        }
    }
    fprintf(file_out, "\n%s\n", b->current->currentPage->description);
}

void PRINT_HISTORY(Browser b, int Id, FILE *file_out){
    list loop_tabs = b->list->next;
    int found = 0;
    //Iterez pana ajung la tab-ul cu id-ul Id
    while(loop_tabs != b->list){
        if(loop_tabs->crt->id == Id){
            found = 1;
            break;
        }
        loop_tabs = loop_tabs->next;
    }
    if(found == 0){
        fprintf(file_out, "403 Forbidden\n");
    }
    else{
        //Afisez paginile de pe stiva forwardStack
        Stack new_stack = loop_tabs->crt->forwardStack;
        while(new_stack != NULL){
            fprintf(file_out, "%s\n", new_stack->crtPage->url);
            new_stack = new_stack->next;
        }
        //Afisez pagina de pe tab-ul curent
        fprintf(file_out, "%s\n", loop_tabs->crt->currentPage->url);
        //Afisez paginile de pe stiva backwardStack
        Stack s = NULL, p = loop_tabs->crt->backwardStack;
        while(p != s){
            if(p->next == s){
                fprintf(file_out, "%s\n", p->crtPage->url);
                s = p;
                p = loop_tabs->crt->backwardStack;
            }
            else{
                p = p->next;
            }
        }
    }
}

int main(){
    FILE *file_in = fopen("tema1.in", "r");
    FILE *file_out = fopen("tema1.out", "w");
    ///pagini = nr de pagini, n este numarul de operatii
    //id_local tine minte id-ul precedent
    int pagini, i, n, id_local = 0, nr;
    ///p este head-ul paginilor
    //cu tmp_page si new_page contruiesc lista de pagini
    Pages p, tmp_page, new_page;
    char tmp[1001], sir[51];
    fscanf(file_in, "%d", &pagini);
    if(pagini == 0){
        p = NULL;
    }
    else if(pagini >= 1){
        //Construiesc head-ul paginilor
        p = malloc(sizeof(struct pages));
        p->crt = malloc(sizeof(struct page));
        fscanf(file_in, "%d", &p->crt->ID);
        fgetc(file_in);
        fgets(p->crt->url, 50, file_in);
        //Pun terminatorul in locul newline-ului in description si url
        p->crt->url[strlen(p->crt->url) - 1] = '\0';
        fgets(tmp, 1001, file_in);
        tmp[strlen(tmp) - 1] = '\0';
        p->crt->description = malloc(strlen(tmp) + 1);
        strcpy(p->crt->description, tmp);
        //Citesc restul paginilor din fisier
        for(i = 2; i <= pagini; i++){
            if(i == 2){
                new_page = citire(p, file_in);
            }
            else{
                tmp_page = citire(new_page, file_in);
                new_page = tmp_page;
            }
        }
        new_page->next = NULL;
    }
    fscanf(file_in, "%d", &n);
    fgetc(file_in);
    Browser b = init_browser();
    b->current->backwardStack = b->current->forwardStack = NULL;
    ///Fac toate operatiile
    for(i = 1; i <= n; i++){
        //Citesc optiunea
        fgets(sir, 51, file_in);
        if(i != n){
            sir[strlen(sir) - 1] = '\0';
        }
        if(strncmp(sir, "NEW_TAB", 7) == 0){
            id_local++;
            NEW_TAB(b, id_local);
        }
        else if(strncmp(sir, "CLOSE", 5) == 0){
            CLOSE(b, file_out);
        }
        else if(strncmp(sir, "OPEN", 4) == 0){
            //Gasesc numarul de la finalul lui OPEN
            nr = strlen(sir) - 1;
            while(sir[nr] != ' '){
                nr--;
            }
            nr++;
            strcpy(sir, sir + nr);
            nr = atoi(sir);
            OPEN(b, nr, file_out);
        }
        else if(strncmp(sir, "NEXT", 4) == 0){
            NEXT(b);
        }
        else if(strncmp(sir, "PREV", 4) == 0){
            PREV(b);
        }
        else if(strncmp(sir, "PAGE", 4) == 0){
            //Gasesc numarul de la finalul lui PAGE
            nr = strlen(sir) - 1;
            while(sir[nr] != ' '){
                nr--;
            }
            nr++;
            strcpy(sir, sir + nr);
            nr = atoi(sir);
            PAGE(b, nr, p, file_out);
        }
        else if(strncmp(sir, "PRINT_HISTORY", 12) == 0){
            //Gasesc numarul de la finalul lui PRINT_HISTORY
            nr = strlen(sir) - 1;
            while(sir[nr] != ' '){
                nr--;
            }
            nr++;
            strcpy(sir, sir + nr);
            nr = atoi(sir);
            PRINT_HISTORY(b, nr, file_out);
        }
        else if(strncmp(sir, "PRINT", 5) == 0){
            PRINT(b, file_out);
        }
        else if(strncmp(sir, "BACKWARD", 8) == 0){
            BACKWARD(b, file_out);
        }
        else if(strncmp(sir, "FORWARD", 7) == 0){
            FORWARD(b, file_out);
        }
    }
    free_browser(b);
    new_page = p;
    if(pagini >= 1){
        free_pages(p);
    }
    fclose(file_in);
    fclose(file_out);
}