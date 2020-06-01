f(0,Res,_,Res). 
f(N,N1,N2,Res):- 
Res1 is N1 + N2, 
Tmp is N - 1, 
f(Tmp,N2,Res1,Res). 
fib(N,Res):- 
f(N,0,1,Res),!.