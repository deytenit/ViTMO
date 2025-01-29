module HW4.T1
  ( EvaluationError (..)
  , ExceptState (..)
  , mapExceptState
  , wrapExceptState
  , joinExceptState
  , modifyExceptState
  , throwExceptState
  , eval
  ) where

import qualified Control.Monad (ap)

import HW4.Types

-- | The 'ExceptState' monad encapsulates computations that can fail with an error
-- of type @e@ and maintain a state of type @s@, producing a result of type @a@.
data ExceptState e s a = ES { runES :: s -> Except e (Annotated s a) }

-- | Maps a function over the result of an 'ExceptState' result.
mapExceptState :: (a -> b) -> ExceptState e s a -> ExceptState e s b
mapExceptState f state = ES $ \s -> case runES state s of
  Success (x :# s') -> Success (f x :# s')
  Error e           -> Error e

-- | Wraps a pure value in the 'ExceptState' monad.
wrapExceptState :: a -> ExceptState e s a
wrapExceptState x = ES $ \s -> Success (x :# s)

-- | Joins two nested 'ExceptState' computations into one.
joinExceptState :: ExceptState e s (ExceptState e s a) -> ExceptState e s a
joinExceptState state = ES $ \s -> case runES state s of
  Success (state' :# s') -> runES state' s'
  Error e                -> Error e

-- | Modifies state in the 'ExceptState' monad.
modifyExceptState :: (s -> s) -> ExceptState e s ()
modifyExceptState f = ES $ \s -> Success (() :# f s)

-- | Throws an error within the 'ExceptState' monad.
throwExceptState :: e -> ExceptState e s a
throwExceptState e = ES $ \_ -> Error e

instance Functor (ExceptState e s) where
  fmap = mapExceptState

instance Applicative (ExceptState e s) where
  pure = wrapExceptState
  (<*>) = Control.Monad.ap

instance Monad (ExceptState e s) where
  (>>=) x y = joinExceptState (fmap y x)

-- | 'EvaluationError' represents possible errors during expression evaluation.
data EvaluationError = DivideByZero
  deriving (Show)

-- | Evaluates an expression within the 'ExceptState' monad, handling possible errors
-- and maintaining a trace of all individual operations.
eval :: Expr -> ExceptState EvaluationError [Prim Double] Double
eval (Val x)        = pure x
eval (Op (Add x y)) = evalBinary (pureModify Add) (+) x y
eval (Op (Sub x y)) = evalBinary (pureModify Sub) (-) x y
eval (Op (Mul x y)) = evalBinary (pureModify Mul) (*) x y
eval (Op (Div x y)) = evalBinary (divModify Div) (/) x y
eval (Op (Abs x))   = evalUnary Abs abs x
eval (Op (Sgn x))   = evalUnary Sgn signum x

-- | Modifies the state of binary operation by pushing it into the trace.
pureModify :: (Double -> Double -> Prim Double) -> Double -> Double -> ExceptState EvaluationError [Prim Double] ()
pureModify op x y = modifyExceptState (op x y :)

-- | Modifies the state for division operation by pushing it into the trace,
-- with error handling for division by zero.
divModify :: (Double -> Double -> Prim Double) -> Double -> Double -> ExceptState EvaluationError [Prim Double] ()
divModify op x y
  | y == 0    = throwExceptState DivideByZero
  | otherwise = pureModify op x y

-- | Evaluates a binary operation, applying a modification function,
-- and evaluation function to the values.
evalBinary :: (Double -> Double -> ExceptState EvaluationError [Prim Double] ())
           -> (Double -> Double -> Double)
           -> Expr -> Expr -> ExceptState EvaluationError [Prim Double] Double
evalBinary modify f x y = do
  x' <- eval x
  y' <- eval y
  modify x' y'
  pure (f x' y')

-- | Evaluates a unary operation, applying a modification function,
-- and evaluation function to the value.
evalUnary :: (Double -> Prim Double)
          -> (Double -> Double)
          -> Expr -> ExceptState EvaluationError [Prim Double] Double
evalUnary op f x = do
  x' <- eval x
  modifyExceptState (op x' :)
  pure (f x')
