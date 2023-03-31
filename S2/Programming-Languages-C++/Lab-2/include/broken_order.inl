#if !defined(FIELD) || !defined(ALPHA)
#error FIELD and ALPHA is not defined.
#else

FIELD(cl_ord_id, token, 9)
FIELD(match_number, integer, 23)
FIELD(reason, reason, 27)
ALPHA(mmt, mmt, 28, 1)  // Trading mode.
ALPHA(mmt, mmt, 29, 2)  // Transaction.
ALPHA(mmt, mmt, 30, 10) // Algo.

#undef FIELD
#undef ALPHA
#endif