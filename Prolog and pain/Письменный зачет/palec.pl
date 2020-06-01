sum(List,Res):-
	s(List,0,Res),!.
s([L|List],N,Res):-
	N1 is N + L,
	s(List,N1,Res).
s([],Res,Res).