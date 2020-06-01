memb([X|Xs],X):-!.
memb([_|Xs],X):-
	memb(Xs,X).