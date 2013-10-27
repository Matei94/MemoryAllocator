MemoryManagement
================

	***	Logica problemei	***

Pentru implementarea mai usoara a functiilor cerute, s-au creat
3 vectori: urm (URMatorul), pre(PREcedentul) si dim(DIMensiunea).
Acesti vectori au functionalitatea unor zone de gestiune. Vectorii 
dati memoreaza pentru fiecare bloc alocat, zona de unde incepe urma-
torul bloc, de unde incepe precentul, respectiv dimensiunea blocului dat.
Acesti vectori sunt vectori ordonati. Adica urm[x], pre[x], si dim[x]
vor contine datele de gestiune ale al x-ului bloc alocat.

Indexul de start este considerat bloc de gestiune. El va retine 
doar adresa de unde incepe primul bloc alocat. Aceasta adresa este
memorata in urm[0] iar dimensiunea indexului de start, adica dim[0]
este constant 4.




		***	Explicarea functiilor	***
.

	# void initialize(int N);

Aceasta functie aloca memorie pentru un bloc mare, compact, de memorie,
numit arena. arena este de fapt un vector de N elemente de tip unsigned char
ce va fi considerata memoria disponibila pentru operatiile urmatoare. 


	# void finalize(int N);

Functia finalize elibereaza memoria alocata in prealabil de functia initialize.


	# void dump(int N);

dump face o mapare frumoasa si organizata a vectorului arena.
In acest sens, functia afiseaza valoarea tuturor octetilor din arena,
intr-un mod organizat, cate 16 pe linie.


	# void alloc(int N, int *urm, int *pre, int *dim, int size);

Aceasta functie aloca blocuri de memorie, daca este posibil.

In primul rand, alloc verifica daca arena este goala. Daca este goala, 
va aloca un bloc de memorie de dimensiune size in cazul in care exista 
suficienta memorie.

Daca arena nu este goala va cauta spatii intre 2 blocuri vecine.
Daca gaseste suficient spatiu va aloca un bloc imediat dupa vecinul din
stanga si va realiza actualizarile de vigoare asupra zonelor de gestiune
ale celor 2 blocuri alaturate.

In cazul in care nu s-a gasit spatiu nici intre 2 blocuri vecine,
va cauta spatiu dupa ultimul bloc alocat. Daca exista suficient spatiu
va aloca aici un nou bloc de memorie.

La final, daca nu exista nicaieri in arena o zona unde poate fi alocat
un nou bloc de dimensiune size, functia va afisa valoarea 0.

Comanda va afisa indexul de inceput al sectiunii de date al blocului
alocat in arena sau 0 in cazul in care alocarea nu a fost posibila.


	# void elib(int N, int *urm, int *pre, int *dim, int INDEX);

Functia elib va elibera blocul de memorie al carei dectiuni de date
incepe la pozitia INDEX. elib face in prealabil si modificarile asupra
zonelor de gestiune ale blocurilor alaturate.


	# void fill(int *urm, int *pre, int *dim, int INDEX, int SIZE, int VALUE);

fill va seta la valoarea VALUE, SIZE octeti incepand de la indexul INDEX.
In cazul in care sunt modificate si zonele de gestiune, fill stie sa modifice
valorile vectorilor urm, pre si dim. Se face aceasta modificare deoarece
restul functiilor se bazeaza pe acesti vectori.


	# void showfree(int N, int *urm, int *pre, int *dim, int conditie);

Functia showfree deserveste 3 comenzi: SHOW FREE, SHOW USAGE si SHOW ALLOCATIONS.
Se face distinctia intre acestea prin parametrul conditie ce poate lua valori
1, 2, respectiv 3. Formulele de calcul pentru octetii liberi, ocupati, blocurile libere sau
ocupate se vad usor in codul sursa. 



		***	Implementarea	***
		
Stim ca orice set de comenzi va incepe prin INITIALIZE N. De aceea se citeste prima
comanda si se scoate prin strtok sirul N. N este un numar, doar ca e reprezentat sub forma
de string, asadar facem conversia lui la int prin functia atoi. Se apeleaza apoi functia
initialize(N) pentru alocarea vectorului arena.

Urmeaza citirea comenzilor si asocierea cu functiile lor.

Ultima comanda apelata este FINALIZE, de aceea se iese din bucla infinita la intalnirea
acestei comenzi.

Ultimul pas este eliberarea vectorilor urm, pre, dim, cmd si comanda.

Am considerat ca functiile sunt cele mai importante din program, nu main-ul, de aceea
nu am folosit antete, ci am implementat functiile inaintea main-ului.
