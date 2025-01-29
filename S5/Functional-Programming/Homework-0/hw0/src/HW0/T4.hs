module HW0.T4
  ( fac
  , fib
  , map'
  , repeat'
  ) where

import Data.Function (fix)
import Numeric.Natural (Natural)

repeat' :: a -> [a]
repeat' x = fix (x :)

map' :: (a -> b) -> [a] -> [b]
map' f = fix g
  where
    g _ []         = []
    g rec (x : xs) = f x : rec xs

fib :: Natural -> Natural
fib = fix g (0, 1)
  where
    g _ (a, _) 0 = a
    g rec (a, b) n = rec (b, a + b) (n - 1)

fac :: Natural -> Natural
fac = fix g
  where
    g _ 0   = 1
    g rec n = n * rec (n - 1)
