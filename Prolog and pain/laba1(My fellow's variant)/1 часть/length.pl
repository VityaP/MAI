leng([],0):-!.
leng([_|Xs],N):-leng(Xs,M),N is M+1.

	