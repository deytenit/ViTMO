module HW1.T2
  ( N (..)
  , nplus
  , nmult
  , nsub
  , nFromNatural
  , nToNum
  , ncmp
  , nEven
  , nOdd
  , ndiv
  , nmod
  ) where


import Data.Maybe (fromJust)
import Numeric.Natural


data N = Z | S N


-- Utility

ndivmod :: N -> N -> (N, N)
ndivmod lhs rhs
  | cmp == EQ = (S Z, Z)
  | cmp == LT = (Z, lhs)
  | otherwise = let res = ndivmod (fromJust $ nsub lhs rhs) rhs in
      (nplus (fst res) (S Z), snd res) where
    cmp = ncmp lhs rhs


-- Export

nplus :: N -> N -> N
nplus Z       = id
nplus (S lhs) = S . nplus lhs

nmult :: N -> N -> N
nmult Z _         = Z
nmult (S lhs) rhs = nplus rhs (nmult lhs rhs)

nsub :: N -> N -> Maybe N
nsub lhs Z           = Just lhs
nsub Z _             = Nothing
nsub (S lhs) (S rhs) = nsub lhs rhs

ncmp :: N -> N -> Ordering
ncmp Z Z             = EQ
ncmp Z _             = LT
ncmp _ Z             = GT
ncmp (S lhs) (S rhs) = ncmp lhs rhs

nFromNatural :: Natural -> N
nFromNatural 0 = Z
nFromNatural n = S $ nFromNatural (n - 1)

nToNum :: Num a => N -> a
nToNum Z       = 0
nToNum (S lhs) = 1 + nToNum lhs

nEven :: N -> Bool
nEven Z         = True
nEven (S Z)     = False
nEven (S (S n)) = nEven n

nOdd :: N -> Bool
nOdd = not . nEven

ndiv :: N -> N -> N
ndiv lhs rhs = fst (ndivmod lhs rhs)

nmod :: N -> N -> N
nmod lhs rhs = snd (ndivmod lhs rhs)
