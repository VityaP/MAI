mark([],[]):-!.
mark([Y|T],[Y,Mark,M|N]):-
	get_marks(Y,Mark,Pass),
	Pass = 2,
	M = 'dont pass exams',
	mark(T,N),!;
	get_marks(Y,Mark,Pass),
	M = 'pass exams',
	mark(T,N),!.

check([2|_],2):-!.
check([],1):-!.
check([M|Marks],Pass):-
	check(Marks,Pass).
	
get_marks(Y,Mark,Pass):-
	findall(Mrk,grade(_,Y,_,Mrk),Marks),
	get_sum_mark(Marks,M),
	check(Marks,Pass),
	length(Marks,Len),
	Mark is M / Len.

get_sum_mark([],0).
get_sum_mark([Y|Marks],M):-
	get_sum_mark(Marks,N),
	M is N + Y.

no_repeats([], []):-!.
no_repeats([X|Xs], Ys):-
	member(X, Xs),!,
	no_repeats(Xs, Ys).
no_repeats([X|Xs], [X|Ys]):-
	no_repeats(Xs, Ys).

students_marks(X):-
	findall(Student,grade(_,Student,_,_),Students),
	no_repeats(Students,Studs),
	mark(Studs,X).