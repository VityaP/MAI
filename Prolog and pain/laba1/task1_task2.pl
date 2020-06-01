%член списка
memb([X|Xs],X):-!.
memb([_|Xs],X):-
	memb(Xs,X).
%перестановки списка	
selct(X,[X|Xs],Xs).
selct(X,[Y|Xs],[Y|Zs]):-selct(X,Xs,Zs).

perm(L,[X|P]):-selct(X,L,L1),perm(L1,P).
perm([],[]).
%удаление всех включений элемента из списка
remove([],X,[]):-!.
remove([X|Xs],X,Zs):-
	remove(Xs,X,Zs).
remove([X|Xs],Z,[X|Zs]):-
	X\=Z,
	remove(Xs,Z,Zs).	
%проверка на подсписок списка
prefx([X|Xs],[Y|Ys]):-
	X=Y,
	prefx(Xs,Ys).	
prefx([],_):-!.
sufx(Xs,[_|Ys]):-
	sufx(Xs,Ys).	
sufx(Xs,Xs):-!.
sublst(Xs,Ys):-
	prefx(Ps,Ys),sufx(Xs,Ps),!.
%конкатенация
app([],Zs,Zs):-!.
app([X|Xs],Ys,[X|Zs]):-
	app(Xs,Ys,Zs).
%длина списка
leng([],0):-!.
leng([_|Xs],N):-leng(Xs,M),N is M+1.
%пункт 4
count_member(_,[],0):-!.
count_member(X,[X|List],Number):-
	count_member(X,List,N1),
	Number is N1 + 1,!.
count_member(X,[_|List],Number):-
	count_member(X,List,Number),!.
	
selct(X,[X|Xs],Xs).
selct(X,[Y|Xs],[Y|Zs]):-selct(X,Xs,Zs).

count_member_2(X,List,Number):-
	member(X,List),
	selct(X,List,Rlist),
	count_member(X,Rlist,N1),
	Number is N1 + 1,!.
%пункт 5	
count_max([Y|List],X):-
	get_max([Y|List],Y,X),!.
get_max([],M,M):-!.
	get_max([L|List],M,X):-
	L>M,
	get_max(List,L,X);
	get_max(List,M,X).
	
count_max_2(List,X):-
	sort(List,Result),
	reverse(Result,[X|_]).

%пункт 6
delete_without_one(X,List,Result):-
	count_member(X,List,N),
	N=1,
	Result=List,!;
	selct(X,List,Reslist),
	delete_without_one(X,Reslist,Result).

%часть 2 задание 1
get_average_mark([],0).
get_average_mark([M|Marks],Sum):-
	get_average_mark(Marks,S),
	Sum is M + S.
average_mark(Subject,X):-
	findall(Mark,grade(_,_,Subject,Mark),Marks),
	get_average_mark(Marks,M),
	length(Marks,Len),
	X is M / Len.
%часть 2 задание 2
no_repeats([], []):-!.
no_repeats([X|Xs], Ys):-
	member(X, Xs),!,
	no_repeats(Xs, Ys).
no_repeats([X|Xs], [X|Ys]):-
	no_repeats(Xs, Ys).
	
dont_pass(Group,X):-
	findall(Student,grade(Group,Student,_,2),Students),
	no_repeats(Students,Studs),
	length(Studs,X).
%часть 2 задание 3
dont_pass_subj(Subject,X):-
	findall(Student,grade(_,Student,Subject,2),Students),
	length(Students,X).
	
	
	
	