remove([],X,[]):-!.
remove([X|Xs],X,Zs):-
	remove(Xs,X,Zs).
remove([X|Xs],Z,[X|Zs]):-
	X\=Z,
	remove(Xs,Z,Zs).	