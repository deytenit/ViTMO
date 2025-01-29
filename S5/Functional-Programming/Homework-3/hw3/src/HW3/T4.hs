module HW3.T4
  ( State (..)
  , Prim (..)
  , Expr (..)
  , mapState
  , wrapState
  , joinState
  , modifyState
  , eval
  ) where

import HW3.T1

newtype State s a = S { runS :: s -> Annotated s a }

mapState :: (a -> b) -> State s a -> State s b
mapState f x = S (mapAnnotated f . runS x)

wrapState :: a -> State s a
wrapState x = S (x :#)

joinState :: State s (State s a) -> State s a
joinState x = S (\s -> let (x' :# s') = runS x s in runS x' s')

modifyState :: (s -> s) -> State s ()
modifyState f = S (\s -> () :# f s)

instance Functor (State s) where
  fmap = mapState

instance Applicative (State s) where
  pure = wrapState
  (<*>) f x = S (\s ->
    let (f' :# s')  = runS f s
        (x' :# s'') = runS x s'
    in  f' x' :# s'')

instance Monad (State s) where
  (>>=) m f = joinState (mapState f m)

data Prim a =
    Add a a
  | Sub a a
  | Mul a a
  | Div a a
  | Abs a
  | Sgn a
  deriving Show

data Expr = Val Double | Op (Prim Expr)
  deriving Show

instance Num Expr where
  (+) x y = Op (Add x y)
  (-) x y = Op (Sub x y)
  (*) x y = Op (Mul x y)
  abs x = Op (Abs x)
  signum x = Op (Sgn x)
  fromInteger x = Val (fromInteger x)

instance Fractional Expr where
  (/) x y = Op (Div x y)
  fromRational x = Val (fromRational x)

eval :: Expr -> State [Prim Double] Double
eval (Val x)        = pure x
eval (Op (Add x y)) = evalBinary Add (+) x y
eval (Op (Sub x y)) = evalBinary Sub (-) x y
eval (Op (Mul x y)) = evalBinary Mul (*) x y
eval (Op (Div x y)) = evalBinary Div (/) x y
eval (Op (Abs x))   = evalUnary Abs abs x
eval (Op (Sgn x))   = evalUnary Sgn signum x

evalBinary :: (Double -> Double -> Prim Double) -> (Double -> Double -> Double) -> Expr -> Expr -> State [Prim Double] Double
evalBinary op f x y = do
  x' <- eval x
  y' <- eval y
  modifyState (op x' y' :)
  pure (f x' y')

evalUnary :: (Double -> Prim Double) -> (Double -> Double) -> Expr -> State [Prim Double] Double
evalUnary op f x = do
  x' <- eval x
  modifyState (op x' :)
  pure (f x')
