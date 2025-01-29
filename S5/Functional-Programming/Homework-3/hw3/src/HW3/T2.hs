module HW3.T2
  ( distOption
  , wrapOption
  , distPair
  , wrapPair
  , distQuad
  , wrapQuad
  , distAnnotated
  , wrapAnnotated
  , distExcept
  , wrapExcept
  , distPrioritised
  , wrapPrioritised
  , distStream
  , wrapStream
  , distList
  , wrapList
  , distFun
  , wrapFun
  ) where

import HW3.T1

distOption :: (Option a, Option b) -> Option (a, b)
distOption (Some x, Some y) = Some (x, y)
distOption _                = None

wrapOption :: a -> Option a
wrapOption = Some

distPair :: (Pair a, Pair b) -> Pair (a, b)
distPair (P lx ly, P rx ry) = P (lx, rx) (ly, ry)

wrapPair :: a -> Pair a
wrapPair x = P x x

distQuad :: (Quad a, Quad b) -> Quad (a, b)
distQuad (Q lw lx ly lz, Q rw rx ry rz) = Q (lw, rw) (lx, rx) (ly, ry) (lz, rz)

wrapQuad :: a -> Quad a
wrapQuad x = Q x x x x

distAnnotated :: Semigroup e => (Annotated e a, Annotated e b) -> Annotated e (a, b)
distAnnotated (x :# xe, y :# ye) = (x, y) :# xe <> ye

wrapAnnotated :: Monoid e => a -> Annotated e a
wrapAnnotated x = x :# mempty

distExcept :: (Except e a, Except e b) -> Except e (a, b)
distExcept (Success x, Success y) = Success (x, y)
distExcept (Error e, _)           = Error e
distExcept (_, Error e)           = Error e

wrapExcept :: a -> Except e a
wrapExcept = Success

distPrioritised :: (Prioritised a, Prioritised b) -> Prioritised (a, b)
distPrioritised (High x, y)    = High (x, unwrapPrioritised y)
distPrioritised (x, High y)    = High (unwrapPrioritised x, y)
distPrioritised (Medium x, y)  = Medium (x, unwrapPrioritised y)
distPrioritised (x, Medium y)  = Medium (unwrapPrioritised x, y)
distPrioritised (Low x, Low y) = Low (x, y)

unwrapPrioritised :: Prioritised a -> a
unwrapPrioritised (Low x)    = x
unwrapPrioritised (Medium x) = x
unwrapPrioritised (High x)   = x

wrapPrioritised :: a -> Prioritised a
wrapPrioritised = Low

distStream :: (Stream a, Stream b) -> Stream (a, b)
distStream (x :> xs, y :> ys) = (x, y) :> distStream (xs, ys)

wrapStream :: a -> Stream a
wrapStream x = x :> wrapStream x

distList :: (List a, List b) -> List (a, b)
distList (_, Nil)                  = Nil
distList (Nil, _)                  = Nil
distList (x :. xs, ys) = let
  cartesian _ Nil         = Nil
  cartesian x' (y :. ys') = (x', y) :. cartesian x ys'
  append Nil ys'         = ys'
  append (x' :. xs') ys' = x' :. append xs' ys' in
    append (cartesian x ys) (distList (xs, ys))

wrapList :: a -> List a
wrapList x = x :. Nil

distFun :: (Fun i a, Fun i b) -> Fun i (a, b)
distFun (F f, F g) = F (\x -> (f x, g x))

wrapFun :: a -> Fun i a
wrapFun x = F (const x)
