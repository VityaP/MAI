uniq([]):- !.
uniq([X|T]):-
	notm(X,T),
	uniq(T).
notm(X,[]).
notm(X,[Y|T]):- 
	X\=Y,
	notm(X,T).