proc(L,N):- length(L,P),Len=P,indx(L,N,P),N\=0,!.
indx([],0,_):-!.
indx([X|L],N,P):- X>=0,indx(L,N,P).
indx([X|L],N,P):- X<0,!,indx([],0,N),length(L,G),N is P-G.