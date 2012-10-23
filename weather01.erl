-module(weather01).
-author("jianchuan.gys@taobao.com").

-export([run/0, run/1, gao/1, gao/0, spawn_run/0, spawn_gao/0]).

-define(PREFIX, "/home/jianchuan.gys/weather/").

run() ->
	timer:tc(fun gao/0).

run(City) ->
	timer:tc(fun gao/1, [City]).

spawn_run() ->
	timer:tc(fun spawn_gao/0).

gao() ->
	{ok, Citys} = file:list_dir(?PREFIX),

	Result = lists:map(
		fun (City) ->  
			gao(City)
		end
	, Citys),
	io:format("~p~n", [Result]).

spawn_gao() ->
	{ok, Citys} = file:list_dir(?PREFIX),

	Self = self(),
	lists:foreach(
		fun (City) ->  
			spawn( fun() -> spawn_gao(City, Self) end )
		end
	, Citys),
	rec(length(Citys), []).

rec(0, Result) ->
	io:format("~p~n", [Result]);
rec(L, Result) ->
	Ret = receive
		Res -> Res
	end,
	rec(L - 1, [Ret | Result]).

spawn_gao(City, From) ->
	From ! gao(City).


gao(City) ->
	Base = string:concat(string:concat(?PREFIX, City), "/"),

	{ok, Days} = file:list_dir(Base),
	Ets = ets:new(list_to_atom(City), [set, private]),
	lists:foreach(
		fun (Day) ->
			% io:format("~p~n", [timer:tc(file, read_file, [string:concat(Base, Day)])]),
			{ok, Content} = file:read_file(string:concat(Base, Day)),
			[RawMax | [RawMin | _]] = binary:split(Content, <<" ">>),
			Max = list_to_integer(binary_to_list(binary:part(RawMax, 4, byte_size(RawMax) - 4))),
			Min = list_to_integer(binary_to_list(binary:part(RawMin, 4, byte_size(RawMin) - 4))),
			Key = list_to_atom(string:sub_string(Day, 1, 7)),
			% io:format("~p~n", [timer:tc(ets, lookup, [Ets, Key])]),
			Val = case ets:lookup(Ets, Key) of
				[{_, B}] -> B;
				_ 		 -> 0
			end,
			% io:format("~p~n", [timer:tc(ets, insert, [Ets, {Key, Val + Max - Min}])]),
			ets:insert(Ets, {Key, Val + Max - Min})
		end
	, Days),
	Ans = solve(ets:tab2list(Ets)),
	{City, Ans}.

solve(List) ->
	do_solve(List, {0, -1}, {0, -1}, {0, -1}, {0, -1}).

do_solve([], {D28, D28_V}, {D29, D29_V}, {D30, D30_V}, {D31, D31_V}) ->
	Max1 = do_max({D28, D28_V / 28}, {D29, D29_V / 29}),
	Max2 = do_max(Max1, {D30, D30_V / 30}),
	do_max(Max2, {D31, D31_V / 31});
do_solve([{D, V} | T], {D28, D28_V}, {D29, D29_V}, {D30, D30_V}, {D31, D31_V}) ->
	case days(D) of
		28 -> do_solve(T, do_max({D28, D28_V}, {D, V}), {D29, D29_V}, {D30, D30_V}, {D31, D31_V});
		29 -> do_solve(T, {D28, D28_V}, do_max({D29, D29_V}, {D, V}), {D30, D30_V}, {D31, D31_V});
		30 -> do_solve(T, {D28, D28_V}, {D29, D29_V}, do_max({D30, D30_V}, {D, V}), {D31, D31_V});
		31 -> do_solve(T, {D28, D28_V}, {D29, D29_V}, {D30, D30_V}, do_max({D31, D31_V}, {D, V}))
	end.

days(D) ->
	SS = string:tokens(atom_to_list(D), "-"),
	{Year, _} 	= string:to_integer(hd(SS)),
	{Month, _} 	= string:to_integer(hd(tl(SS))),
	case Month of
		1 -> 31;
		2 -> case Year of
			1996 -> 29;
			2000 -> 29;
			2004 -> 29;
			2008 -> 29;
			2012 -> 29;
			_ 	 -> 28
		end; 
		3 -> 31;
		4 -> 30;
		5 -> 31;
		6 -> 30;
		7 -> 31;
		8 -> 31;
		9 -> 30;
		10 -> 31;
		11 -> 30;
		12 -> 31
	end.

do_max({D1, V1}, {D2, V2}) ->
	case V1 > V2 of
		true 	-> {D1, V1};
		false 	-> {D2, V2}
	end.
