module HW2.T2
  ( joinWith
  , splitOn
  ) where

import Data.List.NonEmpty (NonEmpty ((:|)))

splitOn :: Eq a => a -> [a] -> NonEmpty [a]
splitOn sep xs = case break (sep==) xs of
  (token, []) -> token :| []
  (token, _:xs') -> let (h :| hs) = splitOn sep xs' in
    token :| (h:hs)


joinWith :: a -> NonEmpty [a] -> [a]
joinWith sep hs =
  foldr1 (\token xs -> token ++ [sep] ++ xs) hs

