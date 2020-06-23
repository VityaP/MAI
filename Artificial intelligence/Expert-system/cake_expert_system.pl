main :-
delete_all,
cake_search(_),nl.


cake_search(Cake) :-
cake(Cake), write(Cake), !.


% Удаление старых ответов
delete_all:-retract(choice(_,_)),delete_all.
delete_all.


% Находит ответ из Choices (то есть мы указали индекс и прога ищет название типа торта среди  choices)
get_element_at_index(0, [First|_], First).
get_element_at_index(Index, [_|Rest], Response) :-
Index > 0,
NextIndex is Index - 1,
get_element_at_index(NextIndex, Rest, Response).



% Вывод вопросов из Choices (выводит меню выбора)
menu([], _).
menu([First|Rest], Index) :-
IndexForPrinting is Index + 1,
write(IndexForPrinting), write(") "), answer(First), nl,
NextIndex is Index + 1,
menu(Rest, NextIndex).



% Задает и сохраняет вопрос 
ask_and_save(Question, Answer, Choices) :-
question(Question),%печать вопроса
menu(Choices, 0),
read(ReadIndex),
Index is ReadIndex - 1,
get_element_at_index(Index, Choices, Response),%получили индекс и углубляемся в область под этим индексом (чтобы получить ответ Response)
asserta(choice(Question, Response)),%вносим в память факт выбора ответа на вопрос
Response = Answer.

 

reason(Answer) :-
choice(reason, Answer).
reason(Answer) :-
\+ choice(reason, _),
ask_and_save(reason, Answer, [birth_day, wedding_day, every_day, gift]).


health(Answer) :-
choice(health, Answer).
health(Answer) :-
\+ choice(health, _),
ask_and_save(health, Answer, [allergy, diabetic, no_matter]).


cream(Answer) :-
choice(cream, Answer).
cream(Answer) :-
\+ choice(cream, _),
ask_and_save(cream, Answer, [chocolate_cream, vanilla_cream, sour_cream]).


cake_layers(Answer) :-
choice(cake_layers, Answer).
cake_layers(Answer) :-
\+ choice(cake_layers, _),
ask_and_save(cake_layers, Answer, [air_biscuit, shortbread_dough, choux_dough,sugar_dough]).

%Вопросы к базе
question(reason) :-
write('Choose reason'), nl.

question(health) :-
write('Choose for health reasons'), nl.

question(cream) :-
write('Choose cream type'), nl.

question(cake_layers) :-
write('choice cake layers type'), nl.



% Ответы к reason
answer(birth_day) :-
write('Birth day').

answer(wedding_day) :-
write('Wedding day').

answer(every_day) :-
write('On every day').

answer(gift) :-
write('Gift').




% Ответы к health
answer(allergy) :-
write('Allergy').

answer(diabetic) :-
write('Diabetic').

answer(no_matter) :-
write('No matter').


%Ответы к cream
answer(sour_cream) :-
write('Sour cream').

answer(vanilla_cream) :-
write('Vanilla cream').

answer(chocolate_cream) :-
write('Chocolate cream').




%Ответы для cake_layers
answer(air_biscuit) :-
write('Air biscuit').

answer(shortbread_dough) :-
write('Shortbread dough').

answer(choux_dough) :-
write('Choux dough').

answer(sugar_dough) :-
write('Sugar dough').

























































