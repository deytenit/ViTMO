module HW2.T3
  ( epart
  , mcat
  ) where

mcat :: Monoid a => [Maybe a] -> a
mcat = let
  fromMaybe (Just just) = just
  fromMaybe Nothing     = mempty in
    foldMap fromMaybe

epart :: (Monoid a, Monoid b) => [Either a b] -> (a, b)
epart = let
  fromEither (Left left)   = (left, mempty)
  fromEither (Right right) = (mempty, right) in
    foldMap fromEither
