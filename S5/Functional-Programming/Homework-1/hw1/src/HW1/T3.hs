module HW1.T3
  ( Tree (..)
  , tsize
  , tdepth
  , tmember
  , tinsert
  , tFromList
  ) where

data Meta = M Int Int
  deriving (Show)

data Tree a = Leaf | Branch Meta (Tree a) a (Tree a)
  deriving (Show)


-- Utility

tbalance :: Tree a -> Int
tbalance Leaf                    = 0
tbalance (Branch _ left _ right) = tdepth left - tdepth right


---- Branching

mkBranch :: Tree a -> a -> Tree a -> Tree a
mkBranch left value right = Branch (M size depth) left value right
  where
    size = 1 + tsize left + tsize right
    depth = 1 + max (tdepth left) (tdepth right)


---- Rotations

slRotation :: Tree a -> Tree a
slRotation Leaf = Leaf
slRotation branch@(Branch _ _ _ Leaf) = branch
slRotation (Branch _ left value (Branch _ rleft rvalue rright)) =
  mkBranch (mkBranch left value rleft) rvalue rright

srRotation :: Tree a -> Tree a
srRotation Leaf = Leaf
srRotation branch@(Branch _ Leaf _ _) = branch
srRotation (Branch _ (Branch _ lleft lvalue lright) value right) =
  mkBranch lleft lvalue (mkBranch lright value right)

blRotation :: Tree a -> Tree a
blRotation Leaf = Leaf
blRotation (Branch _ left value right) = slRotation (mkBranch left value right')
  where right' = srRotation right

brRotation :: Tree a -> Tree a
brRotation Leaf = Leaf
brRotation (Branch _ left value right) = srRotation (mkBranch left' value right)
  where left' = slRotation left

lRotation :: Tree a -> Tree a
lRotation Leaf = Leaf
lRotation branch@(Branch _ _ _ right)
  | tbalance right == 1 = blRotation branch
  | otherwise           = slRotation branch

rRotation :: Tree a -> Tree a
rRotation Leaf = Leaf
rRotation branch@(Branch _ left _ _)
  | tbalance left == -1 = brRotation branch
  | otherwise           = srRotation branch


---- Balance

blBranch :: Tree a -> Tree a
blBranch branch
  | tbalance branch == -2 = lRotation branch
  | tbalance branch == 2  = rRotation branch
  | otherwise             = branch


-- Export

tsize :: Tree a -> Int
tsize Leaf                      = 0
tsize (Branch (M size _) _ _ _) = size

tdepth :: Tree a -> Int
tdepth Leaf                       = 0
tdepth (Branch (M _ depth) _ _ _) = depth

tmember :: Ord a => a -> Tree a -> Bool
tmember _ Leaf = False
tmember x (Branch _ left value right)
  | x < value     = tmember x left
  | x > value     = tmember x right
  | otherwise     = True

tinsert :: Ord a => a -> Tree a -> Tree a
tinsert x Leaf = mkBranch Leaf x Leaf
tinsert x branch@(Branch _ left value right)
  | x < value     = blBranch $ mkBranch (tinsert x left) value right
  | x > value     = blBranch $ mkBranch left value (tinsert x right)
  | otherwise     = branch

tFromList :: Ord a => [a] -> Tree a
tFromList = foldl (flip tinsert) Leaf
