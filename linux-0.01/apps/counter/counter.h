
//ovo ce da se pobrine da se kod u ovom fajlu ne pojavi vise puta
//posto je moguce da vise .c fajlova uradi include ovoga, kao i
//da drugi .h fajlovi rade include ovoga

//ono sto je bitno je da se linije u ovom fajlu pojave jednom
//negde u konacnom proizvodu
#ifndef COUNTER_H_INCLUDED
#define COUNTER_H_INCLUDED

//ovde se NE alocira memorija za glob_counter - to je u counter.c, ovde samo cinimo promenljivu vidljivom u svim fajlovima koji include-uju ovaj .h
//ako zakomentarisemo ovu liniju, counter_test.c ce imati gresku pri direktnom pristupu ovoj promenljivoj
extern int glob_counter;

//funkcije automatski imaju "extern" - ne moramo da pisemo kljucnu rec
void increment_counter();
void decrement_counter();
void print_counter();

#endif //COUNTER_H_INCLUDED
