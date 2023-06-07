prime(N) :- N \= 1, \+ composite(N).

mem(I, MAX_N) :- 
		I =< MAX_N,
		prime(I),
		Next is I * I,
		mem(I, Next, MAX_N).
mem(I, MAX_N) :-
		I =< MAX_N,
		Next is I + 1,
		mem(Next, MAX_N).
mem(Inc, I, MAX_N) :-
		I =< MAX_N,
		assertz(composite(I)),
		Next is I + Inc,
		mem(Inc, Next, MAX_N).

init(MAX_N) :- (mem(2, MAX_N); true).

evaluate_divisors(1, []) :- !.
evaluate_divisors(N, [H1, H2 | T]) :-
		H1 =< H2,
		evaluate_divisors(Divd, [H2 | T]),
		N is Divd * H1.
evaluate_divisors(N, [H]) :-
		evaluate_divisors(Divd, []),
		N is Divd * H.

enumerate_divisors(N, Divr, [N]) :- Divr * Divr > N, !. 
enumerate_divisors(N, Divr, [Divr | T]) :-
		Divr =< N,
		0 is mod(N, Divr),
		Quot is div(N, Divr),
		enumerate_divisors(Quot, Divr, T).
enumerate_divisors(N, Divr, Divisors) :-
		Divr < N,
		\+ (0 is mod(N, Divr)),
		Next is Divr + 1,
		enumerate_divisors(N, Next, Divisors).

prime_divisors(1, []) :- !.
prime_divisors(N, Divisors) :- number(N), enumerate_divisors(N, 2, Divisors).
prime_divisors(N, Divisors) :- \+ number(N), evaluate_divisors(N, Divisors).

evaluate_compact_divisors(1, []) :- !.
evaluate_compact_divisors(N, [(X1, Y1), (X2, Y2) | T]) :-
		X1 < X2,
		evaluate_compact_divisors(Divd, [(X2, Y2) | T]),
		N is Divd * round(X1 ** Y1).
evaluate_compact_divisors(N, [(X, Y)]) :-
		evaluate_compact_divisors(Divd, []),
		N is Divd * round(X ** Y).

enumerate_compact_divisors(N, I, 0, [(N, 1)]) :- I * I > N, !. 
enumerate_compact_divisors(N, Divr, Cnt, Divisors) :-
		Divr =< N,
		0 is mod(N, Divr),
		Quot is div(N, Divr),
		Cnt_Next is Cnt + 1,
		enumerate_compact_divisors(Quot, Divr, Cnt_Next, Divisors).
enumerate_compact_divisors(N, Divr, Cnt, [(Divr, Cnt) | T]) :-
		Divr < N,
		\+ (0 is mod(N, Divr)),
		Cnt > 0,
		Next is Divr + 1,
		enumerate_compact_divisors(N, Next, 0, T).
enumerate_compact_divisors(N, Divr, Cnt, [(Divr, Cnt)]) :-
		Divr > N,
		\+ (0 is mod(N, Divr)),
		Cnt > 0.
enumerate_compact_divisors(N, Divr, Cnt, Divisors) :-
		Divr < N,
		\+ (0 is mod(N, Divr)),
		Cnt is 0,
		Next is Divr + 1,
		enumerate_compact_divisors(N, Next, Cnt, Divisors).

compact_prime_divisors(1, []) :- !.
compact_prime_divisors(N, Divisors) :- number(N), enumerate_compact_divisors(N, 2, 0, Divisors).
compact_prime_divisors(N, Divisors) :- \+ number(N), evaluate_compact_divisors(N, Divisors).

