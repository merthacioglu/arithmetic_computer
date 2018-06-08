//
//  main.c
//  veriYapilariFinal
//
//  Created by mert hacioglu on 25.05.2018.
//  Copyright © 2018 mert hacioglu. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define boyut 50

struct yiginyapisi{
    char *depo[boyut];
    int indis;
};

struct sayiYigin{
    int dizi[boyut];
    int indis;
};

bool push(struct yiginyapisi*, char*);
bool push2(struct sayiYigin*, int);
char *pop(struct yiginyapisi*);
int pop2(struct sayiYigin*);
void stringAyir(char *, char**);
int infixToPostfix(char** ,char**, struct yiginyapisi*);
int postfixCoz(char**, int*, struct sayiYigin*, int, char*);
char **readFile(int*);



bool push(struct yiginyapisi *yigin, char *item){
    if (yigin->indis != boyut){
        yigin->depo[yigin->indis++] = item;
        return true;
    }
    else{
        puts("Yigin dolu oldugu icin ekleme yapilamadi.");
        return false;
    }
}


bool push2(struct sayiYigin *yigin, int item){
    if (yigin->indis != boyut){
        yigin->dizi[yigin->indis++] = item;
        return true;
    }
    else{
        puts("Yigin dolu oldugu icin ekleme yapilamadi.");
        return false;
    }
}


char *pop(struct yiginyapisi *yigin){
    if (yigin->indis != 0)
        return yigin->depo[--yigin->indis];
    else {
        puts("Yiginda cikarilacak eleman yok.");
        return NULL;
    }
}


int pop2(struct sayiYigin *yigin){
    if (yigin->indis != 0)
        return yigin->dizi[--yigin->indis];
    else {
        puts("Yiginda cikarilacak eleman yok.");
        return -1;
    }
}

void stringAyir(char *infix, char **infixForm){
    /*
     Bu fonksiyon string halindeki infix ifadesini
     bosluklarla ayrilan her bir degiskenini
     karakter matrisinin bir gozune koyacak sekilde ayirir.
     @param1 kullanicinin girdigi infix ifadesi
     @param2 ifadenin analiz icin koyuldugu degisken
     */
    int i = 0; // indis
    char *token; // operatorun, degiskenin veya sabit sayinin tutuldugi degisken
    token = strtok(infix, " "); // atanacak degisken alinir
    infixForm[i++] = token; // matriste ilk elemana atanir
    while (token != NULL) {
        //printf("%s ",token);
        token = strtok(NULL, " "); // bosluklari ayrac kabul ederek ifadenin elemanlari alinip baska degiskene atilir
        infixForm[i++] = token;
    }
}

char **readFile(int *ifadeler){
    char **infixExpressions = (char**)malloc(sizeof(char*));
    infixExpressions[0] = (char*)malloc(100*sizeof(char));
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL){
        puts("Dosya bulunamadi");
        exit(-1);
    }
    while (!feof(fp)) {
        fgets(infixExpressions[(*ifadeler)++], 100, fp);
        infixExpressions = realloc(infixExpressions, ((*ifadeler)+1) * sizeof(char*));
        infixExpressions[(*ifadeler)] = (char*)malloc(100*sizeof(char));
    }
    return infixExpressions;
}

int infixToPostfix(char **infixForm, char **postfixForm, struct yiginyapisi *yigin){
    puts("INFIX - POSTFIX DONUSUMU: ");
    /*
     Bu fonksiyon formatli halde verilmis infix ifadeden postfix ifadenin elde edilmesini saglar.
     @param1 gelen infix ifadesi
     @param2 postfix ifadenin saklandigi degisken
     @param3 donusum yapilirken kullanilacak yigin yapisi
     */
    int i = 0; // infix ifade uzerinde gezilirken kullanilacak indis
    int j = 0; // postfix ifade uzerinde nereye yazilacagini gosteren indis
    int k; // hata ayiklama icin yigin yazdirilirken kullanilan indis
    bool islem = false; // = sembolu gorulup gorulmedigini belirten
    while (strcmp(infixForm[i], ";\n")){ // infix ifadenin sonuna gelinene kadar uzerinde dolasilir
        if(!strcmp(infixForm[i], " "))
            continue;
        if (!strcmp(infixForm[i], "=")) // = karakterinden sonraki kisimlar dikkate alinir
            islem = true;
        if (islem){
            if (!strcmp(infixForm[i], "a") || !strcmp(infixForm[i], "b") || !strcmp(infixForm[i], "c") || !strcmp(infixForm[i], "d") || !strcmp(infixForm[i], "e"))
                // eger degisken okunduysa postfix ifadeye yazilir
                postfixForm[j++] = infixForm[i];
            else {
                if (!strcmp(infixForm[i], "(")) // eger ( okunduysa yigina basilir
                    push(yigin, infixForm[i]);
                else if (!strcmp(infixForm[i], ")")){ // eger ) okunduysa yiginda ( gorulene kadar olan tum degiskenler yigindan cekilir ve postfix ifadeye yazilir
                    while (strcmp(yigin->depo[yigin->indis-1], "("))
                        postfixForm[j++] = pop(yigin);
                    pop(yigin); // ( yigindan cekilir ama postfix ifadeye yazilmaz
                }
                else if (!strcmp(infixForm[i], "+") || !strcmp(infixForm[i], "-")){ // + ve ya - operatorleri okunduysa
                    if (yigin->indis == 0 || !strcmp(yigin->depo[yigin->indis-1], "("))
                        // yigin bos ise veya ( karakteri yiginin en uzerinde ise okunan deger yigina basilir
                        push(yigin, infixForm[i]);
                    else{
                        // yiginin uzerinde o an * / + veya - isaretleri varsa yigindan cekilir ve postfix ifadeye eklenir.
                        while (yigin->indis != 0 && strcmp(yigin->depo[yigin->indis-1], ")") && strcmp(yigin->depo[yigin->indis-1], "("))
                            postfixForm[j++] = pop(yigin);
                        push(yigin, infixForm[i]); // okunan ifade yigina basilir
                    }
                }
                else if(!strcmp(infixForm[i], "*") || !strcmp(infixForm[i], "/")){
                    // * veya / okunduysa yigin bossa veya yigin uzerinde ( + veya - varsa okunan deger yigina basilir
                    if(yigin->indis ==0 || !strcmp(yigin->depo[yigin->indis-1], "(") || !strcmp(yigin->depo[yigin->indis-1], "+") ||  !strcmp(yigin->depo[yigin->indis-1], "-") )
                        push(yigin, infixForm[i]);
                    else {
                        // eger yiginin uzerinde * veya - varsa yigin bosalaana veya kendinden dusuk oncelikli bir operator yiginin uzerine gelene kadar yigindan cekilip postfix ifadeye eklenir
                        while (yigin->indis !=0  &&strcmp(yigin->depo[yigin->indis-1], ")") && strcmp(yigin->depo[yigin->indis-1], "(") && strcmp(yigin->depo[yigin->indis-1], "+") && strcmp(yigin->depo[yigin->indis-1], "-"))
                            postfixForm[j++] = pop(yigin);
                        push(yigin, infixForm[i]); // okunan ifade yigina basilir
                    }
                }
                else if (strcmp(infixForm[i], "=")) // sayi okunduysa postfix ifadeye eklenir
                    postfixForm[j++] = infixForm[i];
            }
            printf("Postfix: ");
            for (k=0; k<j; k++) {
                printf("%s",postfixForm[k]);
            }
            printf("   Yigin:"); // her iterasyonda yigin yazdirilir
            for(k=0; k<yigin->indis; k++)
                printf("%s ",yigin->depo[k]);
        }
        i++;
        printf("\n");
    }
    while (yigin->indis != 0){ // infix ifade bittiginde yiginda kalan degerler postfix ifadeye eklenir
        postfixForm[j++] = pop(yigin);
        printf("Postfix: ");
        for (k=0; k<j; k++) {
            printf("%s",postfixForm[k]);
        }
        printf("   Yigin:"); // her iterasyonda yigin yazdirilir
        for(k=0; k<yigin->indis; k++)
            printf("%s ",yigin->depo[k]);
        printf("\n");
    }
    return j; // postfix ifadenin uzunlugu dondurulur
}

int postfixCoz(char **postfix, int *degiskenler, struct sayiYigin *yigin, int uzunluk, char *atanan){
    puts("\nPOSTIX IFADENIN COZULMESI");
    /*
     Bu fonksiyon postfix ifadenin cozulerek islemin sonucunu hesapalar
     @param1 postfix ifadenin tutuldugu degisken
     @param2 ifade icinde degiskenlerin degerlernin tutuldugu dizi
     @param3 hesaplamalar yapilirken kullanilacak yigin ifadesi
     @param4 postfix ifadenin uzunlugu
     @param5 sonucun atanacagi degisken
     */
    int i, sayi1, sayi2, j;
    for (i=0; i<uzunluk; i++) {
        if (!strcmp(postfix[i], "a"))
            // a degiskeni postfix ifadede okunursa degiskenin degeri yigina eklenir
            push2(yigin, degiskenler[0]);
        else if (!strcmp(postfix[i], "b"))
            // b degiskeni postfix ifadede okunursa degiskenin degeri yigina eklenir
            push2(yigin, degiskenler[1]);
        else if (!strcmp(postfix[i], "c"))
            // c degiskeni postfix ifadede okundugunda degiskenin degeri yigina eklenir
            push2(yigin, degiskenler[2]);
        else if (!strcmp(postfix[i], "d"))
            // d degiskeni postfix ifadede okundugunda degiskenin degeri yigina eklenir
            push2(yigin, degiskenler[3]);
        else if(!strcmp(postfix[i], "e"))
            // e degiskeni postfix ifadede okundugunda degiskenin degeri yigina eklenir
            push2(yigin, degiskenler[4]);
        else if(!strcmp(postfix[i], "+")){
            /* + ifadesi okundugudunda yiginin uzerindeki iki sayi cikarilip toplandiktan
             sonra sonuc yigina eklenir */
            sayi1 = pop2(yigin);
            sayi2 = pop2(yigin);
            sayi2 += sayi1;
            push2(yigin, sayi2);
        }
        else if (!strcmp(postfix[i], "-")){
            /* - ifadesi okundugudunda yiginin uzerindeki iki sayi cikarilip cikarildiktan
             sonra sonuc yigina eklenir */
            sayi1 = pop2(yigin);
            sayi2 = pop2(yigin);
            sayi2 -= sayi1;
            push2(yigin, sayi2);
        }
        else if (!strcmp(postfix[i], "*")){
            /* - ifadesi okundugudunda yiginin uzerindeki iki sayi cikarilip carpildiktan
             sonra sonuc yigina eklenir */
            sayi1 = pop2(yigin);
            sayi2 = pop2(yigin);
            sayi2 *= sayi1;
            push2(yigin, sayi2);
        }
        else if (!strcmp(postfix[i], "/")){
            /* / ifadesi okundugudunda yiginin uzerindeki iki sayi cikarilip bolundukten
             sonra sonuc yigina eklenir */
            sayi1 = pop2(yigin);
            sayi2 = pop2(yigin);
            sayi2 /= sayi1;
            push2(yigin, sayi2);
        }
        else
            // sayi okundugu zaman degeri tam sayiya cevrilip yigina eklenir
            push2(yigin, atoi(postfix[i]));
        printf("Yigin: ");
        for(j=0; j<yigin->indis; j++)
            printf("%d ",yigin->dizi[j]);
        printf("\n");
    }
    
    
    
    if (!strcmp(atanan, "a")){
        // islemin sonucu a ya atanacak ise yiginda son kalan deger dizide a'ya ait olan yere alinir.
        degiskenler[0] = pop2(yigin);
        return degiskenler[0];
    }
    else if (!strcmp(atanan, "b")){
        // islemin sonucu b ye atanacak ise yiginda son kalan deger dizide b'ye ait olan yere alinir.
        degiskenler[1] = pop2(yigin);
        return degiskenler[1];
    }
    else if (!strcmp(atanan, "c")){
        // islemin sonucu c ye atanacak ise yiginda son kalan deger dizide c'ye ait olan yere alinir.
        degiskenler[2] = pop2(yigin);
        return degiskenler[2];
    }
    else if (!strcmp(atanan, "d")){
        // islemin sonucu d ye atanacak ise yiginda son kalan deger dizide d'ye ait olan yere alinir.
        degiskenler[3] = pop2(yigin);
        return degiskenler[3];
    }
    else {
        // islemin sonucu e ye atanacak ise yiginda son kalan deger dizide e'ye ait olan yere alinir.
        degiskenler[4] = pop2(yigin);
        return degiskenler[4];
    }
}

int main(int argc, const char * argv[]) {
    int uzunluk; // postfix e cevrilmis karakter matrisinin uzunlugu
    int i, j; // indis
    int ifadeler = 0;
    int degiskenler['e'-'a'+1] ={0, 0, 0, 0, 0}; // degiskenlerin degerlerinin tutuldugu dizi
    char **infixExpressions; // ilk girilen bosluklu infix ifadenin tutuldugu degisken
    char **infixForm; // infix ifadenin analizi icin elemanlarinin her birinin ayri bir yere konuldugu degisken
    char **postfixForm; // infix ifadenin postfixe cevrildikten sonra tutuldugu degisken
    struct yiginyapisi yigin; // infixten postfixe donusum esnasinda karakter katarlarinin tutuldugu yigin yapisi
    struct sayiYigin yigin2; // postfix ifadeden sonuc hesaplanirken sayilarin saklandigi yigin yapisi
    yigin.indis = 0;
    yigin2.indis = 0 ; // yiginlar henuz bos oldugu icin yiginin isaretcisi 0. indisi gosterir
    puts("Degiskenlerin ilk degerleri: ");
    for (i=0; i<sizeof(degiskenler)/sizeof(int); i++)
        printf("%c=%d,  ", 'a'+i, degiskenler[i]);
    infixExpressions = readFile(&ifadeler);
    for(j=0; j<ifadeler-1; j++){
        printf("\n");
        printf("\nInfix ifade: %s\n",infixExpressions[j]);
        infixForm = (char**)malloc(200*sizeof(char*));
        postfixForm = (char**)malloc(200*sizeof(char*));
        for(i = 0; i<sizeof(infixForm)/sizeof(char*); i++){
            *(infixForm+i) = (char*)malloc(5*sizeof(char));
            *(postfixForm+i) = (char*)malloc(5*sizeof(char));
        } // infix ve postfix ifadenin sonradan saklandigi degiskenler icin yer ayrilir
        stringAyir(infixExpressions[j], infixForm); // string olarak okunan infix ifade daha sonra analiz icin operatorleri, degiskenleri ve sabitleri ayrilmasi icin fonksiyona verilir
        uzunluk = infixToPostfix(infixForm, postfixForm, &yigin); // infix ifade postfix ifadeye cevrilir ve postfix ifadenin uzunlugu dondurulur
        printf("\n%s degiskenindeki islem sonucu : %d", infixForm[0], postfixCoz(postfixForm, degiskenler, &yigin2, uzunluk, infixForm[0]));
        puts("\nDegiskenlerin yeni degerleri: ");
        for (i=0; i<sizeof(degiskenler)/sizeof(int); i++)
            printf("%c=%d,  ", 'a'+i, degiskenler[i]);
        printf("\n\n\n");
        
    }
    return 0;
}


