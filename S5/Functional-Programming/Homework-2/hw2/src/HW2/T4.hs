module HW2.T4
  ( DotString (..)
  , Fun (..)
  , Inclusive (..)
  , ListPlus (..)
  ) where

data ListPlus a = a :+ ListPlus a | Last a
  deriving Show

infixr 5 :+

instance Semigroup (ListPlus a) where
  (<>) (h :+ lhs) rhs = h :+ (lhs <> rhs)
  (<>) (Last a) rhs   = a :+ rhs

data Inclusive a b = This a | That b | Both a b
  deriving Show

instance (Semigroup a, Semigroup b) => Semigroup (Inclusive a b) where
  (<>) (This lhs) (This rhs)             = This $ lhs <> rhs
  (<>) (That lhs) (That rhs)             = That $ lhs <> rhs
  (<>) (Both llhs lrhs) (Both rlhs rrhs) = Both (llhs <> rlhs) (lrhs <> rrhs)
  (<>) (This lhs) (That rhs)             = Both lhs rhs
  (<>) (That lhs) (This rhs)             = Both rhs lhs
  (<>) (This lhs) (Both rlhs rrhs)       = Both (lhs <> rlhs) rrhs
  (<>) (That lhs) (Both rlhs rrhs)       = Both rlhs (lhs <> rrhs)
  (<>) (Both llhs lrhs) (This rhs)       = Both (llhs <> rhs) lrhs
  (<>) (Both llhs lrhs) (That rhs)       = Both llhs (rhs <> lrhs)


newtype DotString = DS String
  deriving Show

instance Semigroup DotString where
  (<>) (DS []) rhs       = rhs
  (<>) lhs (DS [])       = lhs
  (<>) (DS lhs) (DS rhs) = DS $ lhs ++ "." ++ rhs

instance Monoid DotString where
  mempty = DS []

newtype Fun a = F (a -> a)

instance Semigroup (Fun a) where
  (<>) (F f) (F g) = F $ f . g

instance Monoid (Fun a) where
  mempty = F id
