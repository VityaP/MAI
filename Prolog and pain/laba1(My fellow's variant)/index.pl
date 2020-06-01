proc([],0):-!.

proc([X|L],N):- X>=0,proc(L,N1),N is N1+1.
proc([X|L],N):- X<0,!,proc([],N1),N is N1+1.