module HW3.T3
  ( joinOption
  , joinExcept
  , joinAnnotated
  , joinList
  , joinFun
  ) where

import HW3.T1

joinOption :: Option (Option a) -> Option a
joinOption (Some (Some x)) = Some x
joinOption _               = None

joinExcept :: Except e (Except e a) -> Except e a
joinExcept (Success (Success x)) = Success x
joinExcept (Success (Error e))   = Error e
joinExcept (Error e)             = Error e

joinAnnotated :: Semigroup e => Annotated e (Annotated e a) -> Annotated e a
joinAnnotated ((x :# e) :# e') = x :# e' <> e

joinList :: List (List a) -> List a
joinList Nil                = Nil
joinList (Nil :. xs)        = joinList xs
joinList ((x :. xs) :. xs') = x :. joinList (xs :. xs')

joinFun :: Fun i (Fun i a) -> Fun i a
joinFun (F f) = F (\x -> let (F g) = f x in g x)
