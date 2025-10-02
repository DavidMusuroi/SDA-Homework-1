1) Punctajul obtinut pe checker-ul local:

 -= TEMA 1 SDA =-

README: 5/5
01.When_NoPagesProviedAndPrintIsTheOnlyOperation_Should_PrintTab0.in: 5/5
  Valgrind PASSED
02.When_NoPagesProviedAnd3TabsOpen_Should_PrintTabs.in: 5/5
  Valgrind PASSED
03.When_NoPagesProviedAnd3TabsOpenAnd2TabsClosed_Should_PrintTabs.in: 2/2
  Valgrind PASSED
04.When_NoPagesProviedAnd1TabsOpenAnd2TabsClosed_Should_ReturnOnlyError.in: 2/2
  Valgrind PASSED
05.When_NoPagesProviedAnd1TabsOpenAnd2TabsClosedAndPrint_Should_ReturnErrorAndPrintDefaultTab.in: 3/3
  Valgrind PASSED
06.When_NoPagesProviedAnd2TabsOpenAnd1TabsClosedAndPrintAnd1TabsOpenAndPrint_Should_PrintMultipleTimes.in: 3/3
  Valgrind PASSED
07.When_NoPagesProviedAnd2TabsOpenAndOpenAndPrint_Should_PrintInTheCorrectOrder.in: 2/2
  Valgrind PASSED
08.When_NoPagesProvieAndOpenTabThatDoesNotExists_Should_ReturnError.in: 3/3
  Valgrind PASSED
09.When_NoPagesProvieAndMultipleNewTabsAndOpenAndPrint_Should_PrintCorrectOrder.in: 3/3
  Valgrind PASSED
10.When_NoTabsAndNextAndPrint_Should_PrintTabs.in: 3/3
  Valgrind PASSED
11.When_NoTabsAndPrevAndPrint_Should_PrintTabs.in: 3/3
  Valgrind PASSED
12.When_NewTabsAndNextAndPrint_Should_PrintTabs.in: 3/3
  Valgrind PASSED
13.When_NewTabsAndPrevAndPrint_Should_PrintTabs.in: 3/3
  Valgrind PASSED
14.When_NewTabsAndPrevsAndNextsAndPrint_Should_PrintTabs.in: 2/2
  Valgrind PASSED
15.When_NewTabsAndClosesAndPrevsAndNextsAndPrint_Should_PrintTabs.in: 2/2
  Valgrind PASSED
16.When_NewTabsAndClosesAndOpensAndPrevsAndNextsAndPrint_Should_PrintTabs.in: 2/2
  Valgrind PASSED
17.When_PagesAndNewTabsAndClosesAndOpensAndPrevsAndNextsAndPrints_Should_PrintTabs.in: 3/3
  Valgrind PASSED
18.When_NoPagesProviedAndPage_Should_ReturnError.in: 2/2
  Valgrind PASSED
19.When_PagesAndPageThatDoesNotExists_Should_ReturnError.in: 2/2
  Valgrind PASSED
20.When_PagesAndPageAndPrint_Should_PrintCurrentPage.in: 2/2
  Valgrind PASSED
21.When_PagesAnd2NewTabsAndPageAndPrintAndPageAndPrint_Should_PrintMultipleTimes.in: 3/3
  Valgrind PASSED
22.PrintHistory_NoPages_Basic.in: 2/2
  Valgrind PASSED
23.PrintHistory_NoPages_Basic_Error.in: 2/2
  Valgrind PASSED
24.PrintHistory_Pages_Basic.in: 3/3
  Valgrind PASSED
25.Backward_NoPages_Basic_Error.in: 2/2
  Valgrind PASSED
26.Backward_Pages_Basic.in: 2/2
  Valgrind PASSED
27.Backward_Pages_Basic_Error.in: 3/3
  Valgrind PASSED
28.Forward_NoPages_Basic_Error.in: 2/2
  Valgrind PASSED
29.Forward_Pages_Basic.in: 2/2
  Valgrind PASSED
30.Forward_Pages_Basic_Error.in: 3/3
  Valgrind PASSED
31.ComplexTest.in: 2/2
  Valgrind PASSED
32.ComplexTest.in: 2/2
  Valgrind PASSED
33.ComplexTest.in: 2/2
  Valgrind PASSED
34.ComplexTest.in: 2/2
  Valgrind PASSED
35.ComplexTest.in: 2/2
  Valgrind PASSED
36.ComplexTest.in: 2/2
  Valgrind PASSED
37.ComplexTest.in: 2/2
  Valgrind PASSED
38.ComplexTest.in: 2/2
  Valgrind PASSED

Total: 100/100
Valgrind: 20/20

2) Implementarea temei:

1. Am implementat atat stack-ul cat si paginile citite din input sub forma de
liste simplu inlantuite, ambele fiind alocate dinamic.

2. Prima pagina (daca exista) o citesc separat pentru a o initializa drept
head al stivei de pagini.

3. Functia *free_stack* elibereaza stack-urile backwardStack si forwardStack.

4. Functia *free_pages* elibereaza paginile citite din fisier.

5. Functia *free_browser* elibereaza atat pagina implicita, cat si tab-urile
din lista circulara, in final eliberandu-se santinela si browser-ul.

6. Functia *citire* citeste paginile de la 2 incolo din fisier (daca exista).

7. Functia *init_browser* initializeaza tab-ul santinela, primul tab al
browser-ului si pagina implicita a acestuia.

8. Functia *NEW_TAB* itereaza pana la ultimul tab, creaza un nou tab cu pagina
implicita si il adauga la lista circulara.

9. Functia *CLOSE* itereaza pana la ultimul tab, il elimina, restabileste
legaturile din lista circulara si il elibereaza din memorie.

10. Functia *OPEN* itereaza pana la tab-ul cu id-ul corespunzator si fie
afiseaza o eroare, fie il actualizeaza ca fiind tab-ul curent.

11. Functiile *NEXT* si *PREV* itereaza prin lista circulara pana ajung la
tab-ul curent si il actualizeaza ca fiind urmatorul respectiv precedentul tab.

12. Functia *PAGE* sterge mai intai continutul stivei forwardStack folosind
functia *free_stack*, dupa care incearca sa gaseasca id-ul paginii cautate,
afisand eroarea daca nu il gaseste. In caz contrar, construieste primul
element al stivei backwardStack daca aceasta este nula, sau itereaza pana la
finalul ei si o actualizeaza cu pagina care se afla momentan in browser,
reinitializand pagina curenta din browser cu id-ul paginii apelate de functie.

13. Functiile *BACKWARD* si *FORWARD* afiseaza eroarea daca sunt goale in
momentul apelarii, sau itereaza pana la ultima pagina din backwardStack si
forwardStack. In continuare, functia *BACKWARD* introduce in forwardStack
pagina curenta, actualizeaza pagina curenta din browser si elibereaza din
memorie ultima pagina din backwardStack, in timp ce functia *FORWARD*
introduce in backwardStack pagina curenta, actualizeaza pagina curenta din
browser si elibereaza din memorie ultima pagina din forwardStack

14. Functia *PRINT* afiseaza toate tab-urile incepand cu tab-ul curent,
afisand la final descrierea tab-ului curent din browser.

15. Functia *PRINT_HISTORY* afiseaza eroarea daca nu exista tab-ul cu id-ul
apelat de functie, iar in caz contrar afiseaza paginile de pe backwardStack,
pagina curenta si paginile de pe forwardStack in aceasta ordine.

16. In *main*, dupa citirea paginilor si a browser-ului, citesc cele n optiuni
si apelez de fiecare data functia cu acelasi nume, dupa care eliberez memoria
ocupata de browser si de paginile citite, iar in final inchid fisierele de
input si output.
