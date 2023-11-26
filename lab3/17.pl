% Правило для вычисления суммы цифр в числе
сумма_цифр(0, 0).
сумма_цифр(N, Сумма) :-
    N > 0,
    Остаток is N mod 10,
    Новое_Число is N // 10,
    сумма_цифр(Новое_Число, Сумма_Остатка),
    Сумма is Сумма_Остатка + Остаток.

% Правило для вычисления цифрового корня
цифровой_корень(N, Корень) :-
    сумма_цифр(N, Сумма),
    Сумма < 10,
    Корень is Сумма.
цифровой_корень(N, Корень) :-
    сумма_цифр(N, Сумма),
    Сумма >= 10,
    цифровой_корень(Сумма, Корень).

main :-
    Натуральное_число = 123456, 

    цифровой_корень(Натуральное_число, Корень),

    write("Цифровой корень числа "), write(Натуральное_число), write(" равен "), write(Корень), nl.

:- main.