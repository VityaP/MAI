fact(1,1). /* факториал единицы равен единице */
fact(N,F):-
	N>1, /* убедимся, что число больше единицы */
	N1=N-1,
	fact(N1,F1), /* F1 равен факториалу числа, 
	                на единицу меньшего исходного 
	                числа */
	F=F1*N.