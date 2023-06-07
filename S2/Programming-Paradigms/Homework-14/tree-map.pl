% ----------fields----------

value(nil, nil).
left(nil, nil).
right(nil, nil).
height(nil, 0).

value(tree(X, _, _, _), X).
left(tree(_, L, _, _), L).
right(tree(_, _, R, _), R).
height(tree(_, _, _, H), H).

% ----------height----------

eval_height(X, Y, H) :- X > Y,  H is X + 1.
eval_height(X, Y, H) :- X =< Y, H is Y + 1.

reheight(tree(X, L, R, _), tree(X, L, R, H)) :-
	height(L, HL),
	height(R, HR),
	eval_height(HL, HR, H).

% ----------balance----------

trbal(tree(_, L, R, _), Diff) :-
	height(L, HL),
	height(R, HR),
	Diff is HL - HR.

% ----------rotations----------

ls_rotation(tree(X1, L, tree(X2, C, R, _), _), Res) :-
	reheight(tree(X1, L, C, _), L_Res),
	reheight(tree(X2, L_Res, R, _), Res).
rs_rotation(tree(X1, tree(X2, L, C, _), R, _), Res) :-
	reheight(tree(X1, C, R, _), R_Res),
	reheight(tree(X2, L, R_Res, _), Res).

lb_rotation(tree(X, L, R), Res) :- rs_rotation(R, Tmp), ls_rotation(tree(X, L, Tmp), Res).
rb_rotation(tree(X, L, R), Res) :- ls_rotation(L, Tmp), rs_rotation(tree(X, Tmp, R), Res).

l_rotation(tree(X, L, R, H), Res) :- trbal(R, 1), lb_rotation(tree(X, L, R, H), Res), !.
l_rotation(tree(X, L, R, H), Res) :- ls_rotation(tree(X, L, R, H), Res).

r_rotation(tree(X, L, R, H), Res) :- trbal(L, -1), rb_rotation(tree(X, L, R, H), Res), !.
r_rotation(tree(X, L, R, H), Res) :- rs_rotation(tree(X, L, R, H), Res).

% ----------balance----------

balance(nil, nil).
balance(Tree, Res) :- trbal(Tree, -2), l_rotation(Tree, Res), !.
balance(Tree, Res) :- trbal(Tree, 2), r_rotation(Tree, Res), !.
balance(Tree, Tree).

rebalance(Tree, Res) :- reheight(Tree, Tmp), balance(Tmp, Res).

% ----------operations----------

map_min(tree((K, V), nil, _, _), K, V) :- !. 
map_min(tree(_, L, _, _), K_Res, V_Res) :- map_min(L, K_Res, V_Res).

map_max(tree((K, V), _, nil, _), K, V) :- !. 
map_max(tree(_, _, R, _), K_Res, V_Res) :- map_max(R, K_Res, V_Res).

map_get(tree((K, V), _, _, _), K, V) :- !. 
map_get(tree((K1, _), L, _, _), K2, V) :- K2 < K1, map_get(L, K2, V), !.
map_get(tree(_, _, R, _), K2, V) :- map_get(R, K2, V).

map_put(nil, K, V, tree((K, V), nil, nil, 1)).
map_put(tree((K, _), L, R, H), K, V, tree((K, V), L, R, H)) :- !. 
map_put(tree((K1, V1), L, R, _), K2, V2, Res) :-
	K2 < K1,
	map_put(L, K2, V2, L_Res),
	rebalance(tree((K1, V1), L_Res, R, _), Res), !.
map_put(tree((K1, V1), L, R, _), K2, V2, Res) :-
	map_put(R, K2, V2, R_Res),
	rebalance(tree((K1, V1), L, R_Res, _), Res).

map_putIfAbsent(Tree, K, _, Tree) :- map_get(Tree, K, _), !.
map_putIfAbsent(Tree, K, V, Res) :- map_put(Tree, K, V, Res).

map_build([], nil) :- !.
map_build([(K, V) | T], Res) :- map_build(T, Tmp), map_put(Tmp, K, V, Res).

map_remove(nil, _, nil).
map_remove(tree((K, _), nil, nil, _), K, nil) :- !.
map_remove(tree((K, _), L, R, _), K, Res) :-
	trbal(tree(_, L, R, _), -1),
	map_min(R, K_Min, V_Min),
	map_remove(R, K_Min, R_Res),
	rebalance(tree((K_Min, V_Min), L, R_Res, _), Res), !.
map_remove(tree((K, _), L, R, _), K, Res) :-
	map_max(L, K_Max, V_Max),
	map_remove(L, K_Max, L_Res),
	rebalance(tree((K_Max, V_Max), L_Res, R, _), Res), !.
map_remove(tree((K1, V1), L, R, _), K2, Res) :-
	K2 < K1,
	map_remove(L, K2, L_Res),
	rebalance(tree((K1, V1), L_Res, R, _), Res), !.
map_remove(tree((K1, V1), L, R, _), K2, Res) :-
	map_remove(R, K2, R_Res),
	rebalance(tree((K1, V1), L, R_Res, _), Res).