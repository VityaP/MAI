solve(L):-
    L = [['muzh',_],['zhena',_],['syn',_],['sestra muzha',_],['otec zheny',_]],
    member([X,'yurist'],L),  
    member([Y,'uchitel'],L),
    not_blood_relatives(X,Y), 
    member([Z,'slesar'],L),
    member([U,'ehkonomist'],L),
    male(U),
    \+younger(U, Z),
    member([V, 'inzhener'], L),
    brother('muzh', V),
    \+younger(V, 'zhena'),
    \+younger(V, 'zhena'),
    \+younger(Y, V),!.

male('muzh').
male('syn').
male('otec zheny').

brother('muzh', 'sestra muzha').

not_blood_relative('muzh', 'zhena').
not_blood_relative('sestra muzha', 'zhena').
not_blood_relative('otec zheny', 'muzh').
not_blood_relative('sestra muzha', 'otec zheny').

not_blood_relatives(X, Y):- 
    not_blood_relative(X, Y); 
    not_blood_relative(Y, X).

younger('syn', 'muzh').
younger('syn', 'zhena').
younger('syn', 'otec zheny').
younger('zhena', 'otec zheny').
younger('zhena', 'sestra muzha').