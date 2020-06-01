not(X) :- X,!,fail.
not(_).
 
search :- search_id([3,3,left_coast],[0,0,right_coast]).
 
int_g(1).
int_g(N):-int_g(M), N is M+1.
 
id([Finish|Tail],Finish,[Finish|Tail],0).
id(TempWay,Finish,Way,N):-N>0,
    prolong(TempWay,NewWay), N1 is N-1,
    id(NewWay,Finish,Way,N1).
 
 
search_id(Start,Finish):-
    int_g(Level),
    (Level>100,!;
    id([Start],Finish,MoveList,Level),
    output(MoveList)).
 
 
% Recursive call to solve the problem
 
prolong([[A,OutA]|Tail],[[B,OutB],[A,OutA]|Tail]):-
    move(A,B,OutB),legal(B),not(member(B,Tail)).
 
% Move commands and descriptions of the move
 
move([A,B,left_side],[C,B,right_side],'1 missionar crosses the river') :-
   A > 0, C is A - 1.
move([A,B,left_side],[A,D,right_side],'1 cannibal crosses the river') :-
   B > 0, D is B - 1.
move([A,B,right_side],[C,B,left_side],'1 missionar returns from the other side') :-
   A < 3, C is A + 1.
move([A,B,right_side],[A,D,left_side],'1 cannibal returns from the other side') :-
   B < 3, D is B + 1.
move([A,B,left_side],[C,D,right_side],'1 missionar and 1 cannibal cross the river') :-
   A > 0, B > 0, C is A - 1, D is B - 1.
move([A,B,right_side],[C,D,left_side],'1 missionar and 1 cannibal return from the other side') :-
   A < 3, B < 3, C is A + 1, D is B + 1.
move([A,B,left_side],[C,B,right_side],'2 missionars cross the river') :-
   A > 1, C is A - 2.
move([A,B,left_side],[A,D,right_side],'2 cannibals cross the river') :-
   B > 1, D is B - 2.
move([A,B,right_side],[C,B,left_side],'2 missionars return from the other side') :-
   A < 2, C is A + 2.
move([A,B,right_side],[A,D,left_side],'2 cannibals return from the other side') :-
   B < 2, D is B + 2.
move([A,B,left_side],[C,D,right_side],'1 cannibal and 2 missionary cross the river') :-
   A > 1,B > 0, D is B - 1, C is A - 2.
 
% Legal move definition where B is missionaries and A is cannibals:
 
legal([B,A,_]) :-
   (A =< B ; B = 0),
   C is 3-A, D is 3-B,
   (C =< D; D = 0).

