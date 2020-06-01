list_insert(X,1,L,[X|O]):-
	append([],L,O),!.
	
list_insert(X,I,[Y|L],[Y|O]):-
	I1 is I-1,
	list_insert(X,I1,L,O).
get(X,I,N,L,O):-
	change(X,I,N,L,O).
	
change(_,_,0,L,A):-
	append([],L,A),
	!.
change(X,I,N,L,A):-
	list_insert(X,I,L,Z),
	N1 is N-1,
	change(X,I,N1,Z,A).
	