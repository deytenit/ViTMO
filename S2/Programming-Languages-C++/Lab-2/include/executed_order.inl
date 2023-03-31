#if !defined(FIELD) || !defined(ALPHA)
#error FIELD and ALPHA is not defined.
#else

FIELD(cl_ord_id, token, 9)
FIELD(filled_volume, integer, 23)
FIELD(price, price, 27)
FIELD(match_number, integer, 32)
ALPHA(counterpart, token, 36, 4)
ALPHA(mmt, mmt, 40, 1)  // Trading mode.
ALPHA(mmt, mmt, 41, 2)  // Transaction.
ALPHA(mmt, mmt, 42, 10) // Algo.
FIELD(liquidity_indicator, liquidity, 43)
ALPHA(internalized, bit, 43, 5)
ALPHA(self_trade, bit, 43, 7)

#undef FIELD
#undef ALPHA
#endif