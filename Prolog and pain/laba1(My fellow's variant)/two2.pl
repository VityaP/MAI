no_repeats([], []):-!.
no_repeats([X|Xs], Ys):-
	member(X, Xs),!,
	no_repeats(Xs, Ys).
no_repeats([X|Xs], [X|Ys]):-
	no_repeats(Xs, Ys).

count([],0).
count([_|S],Number):-
	count(S,N),
	Number is N + 1.

get_dont_passed([],[]).
get_dont_passed([N|S],[N,Number|X]):-
	findall(N,grade(_,_,N,2),Subj),
	count(Subj,Number),
	get_passed(S,X).
	
dont_pass_exams(X):-
	findall(Subjects,grade(_,_,Subjects,2),Subj),
	no_repeats(Subj,S),
	get_dont_passed(S,X).