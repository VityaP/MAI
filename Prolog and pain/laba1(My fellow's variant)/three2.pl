no_repeats([], []):-!.
no_repeats([X|Xs], Ys):-
	member(X, Xs),!,
	no_repeats(Xs, Ys).
no_repeats([X|Xs], [X|Ys]):-
	no_repeats(Xs, Ys).
	
sum_mark([],0).
sum_mark([Y|Marks],M):-
	sum_mark(Marks,N),
	M is N + Y.
	
calc_max_group_mark([],Result,Max):-
	Result = Max,!.
calc_max_group_mark([M|Marks],Result,Max):-
	M > Max,
	calc_max_group_mark(Marks,Result,M);
	calc_max_group_mark(Marks,Result,Max).
	
get_average_marks([],[]).
get_average_marks([S|Students],[Max|M]):-
	findall(X,grade(_,S,_,X),Marks),
	sum_mark(Marks,N),
	length(Marks,Len),
	Max is N / Len,
	get_average_marks(Students,M).

make_list([],[]).
make_list([G|Groups],[G,Max|Xs]):-
	findall(S,grade(G,S,_,_),Stud),
	no_repeats(Stud,Students),
	get_average_marks(Students,Marks),
	calc_max_group_mark(Marks,Max,0),
	make_list(Groups,Xs).

max_mark(X):-
	findall(B,grade(B,_,_,_),G),
	no_repeats(G,Groups),
	make_list(Groups,X).