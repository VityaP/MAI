list_insert(X,0,[],[]):-!.

list_insert(X,0,[Y|L],[Y|O]):-
	list_insert(X,0,L,O),!.
	
list_insert(X,1,L,[X|O]):-
	list_insert(X,0,L,O),!.
	
list_insert(X,I,[Y|L],[Y|O]):-
	I1 is I-1,
	list_insert(X,I1,L,O).