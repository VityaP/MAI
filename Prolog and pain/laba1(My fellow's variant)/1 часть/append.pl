app([],Zs,Zs):-!.
app([X|Xs],Ys,[X|Zs]):-
	app(Xs,Ys,Zs).