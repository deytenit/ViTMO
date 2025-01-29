{-# LANGUAGE DerivingStrategies         #-}
{-# LANGUAGE GeneralisedNewtypeDeriving #-}

module HW4.T2
  ( ParseError (..)
  , runP
  , pChar
  , parseError
  , parseExpr
  ) where

import Control.Applicative
import Control.Monad
import Data.Char (isDigit, isSpace, ord)
import Data.Functor (($>))
import Numeric.Natural (Natural)

import HW4.T1 (ExceptState (..))
import HW4.Types

-- | Indicating an error at a specific position of the input.
data ParseError = ErrorAtPos Natural
  deriving Show

-- | Wraps an 'ExceptState' which handles parsing state,
-- errors, and resulting values.
newtype Parser a = P (ExceptState ParseError (Natural, String) a)
  deriving newtype (Functor, Applicative, Monad)

-- | Creates a parse error at the current position.
parseError :: Parser a
parseError = P $ ES $ \(pos, _) -> Error (ErrorAtPos pos)

-- | Alternative instance of Parser implements choice between Parsers.
instance Alternative Parser where
  empty = parseError
  (<|>) (P lstate) (P rstate) = P $ ES $ \(pos, s) ->
    case runES lstate (pos, s) of
      Error _ -> runES rstate (pos, s)
      res     -> res

-- | MonoidPlus instance of Parser implements choice between Parsers.
-- It is a stronger claim than indicating that it is both an Alternative and Monad.
instance MonadPlus Parser

-- | Runs Parser on a string provided.
runP :: Parser a -> String -> Except ParseError a
runP (P state) str = case runES state (0, str) of
  Error e            -> Error e
  Success (res :# _) -> Success res

-- | Parses a character that satisfies the given predicate.
-- Advances the parser position if successful.
pChar :: (Char -> Bool) -> Parser Char
pChar prd = P $ ES $ \(pos, s) ->
  case s of
    (c:cs) | prd c -> Success (c :# (pos + 1, cs))
    _              -> Error (ErrorAtPos pos)

-- | Ignores whitespace characters.
pWs :: Parser ()
pWs = void $ many (pChar isSpace)

-- | Successes if Parser has reached the end of the input.
pEof :: Parser ()
pEof = P $ ES $ \(pos, s) ->
  case s of
    [] -> Success (() :# (pos, s))
    _  -> Error (ErrorAtPos pos)

-- | Parses a numeric value as Double and wraps it in Val constructor.
pVal :: Parser Expr
pVal = do
  whole <- fmap toWhole (some $ pChar isDigit)
  fract <- pChar ('.' ==) *> fmap toFract (many $ pChar isDigit) <|> pure 0
  pure $ Val $ whole + fract
    where
      -- Converts a list of digit characters to an integer part of a Double.
      toWhole :: Foldable t => t Char -> Double
      toWhole = foldl (\acc d -> acc * 10 + fromIntegral (ord d - ord '0')) 0

      -- Converts a list of digit characters to a fractional part of a Double.
      toFract :: Foldable t => t Char -> Double
      toFract = foldr (\d acc -> (acc + fromIntegral (ord d - ord '0')) / 10) 0

-- | Parses a term, which could be a parenthesized expression or a numeric value.
pTerm :: Parser Expr
pTerm = (pChar ('(' ==) *> pWs *> pExpr <* pWs <* pChar (')' ==)) <|> pVal

-- | Parses multiplication or division expressions.
pFactor :: Parser Expr
pFactor = pOperation pTerm (pMul <|> pDiv)
  where
    -- Converts operations to the respective Prim Expr.
    pMul = (pWs *> pChar ('*' ==) <* pWs) $> Mul
    pDiv = (pWs *> pChar ('/' ==) <* pWs) $> Div

-- | Parses addition or subtraction expressions.
pExpr :: Parser Expr
pExpr = pOperation pFactor (pAdd <|> pSub)
  where
    -- Converts operations to the respective Prim Expr.
    pAdd = (pWs *> pChar ('+' ==) <* pWs) $> Add
    pSub = (pWs *> pChar ('-' ==) <* pWs) $> Sub

-- | Parses left-associative binary opeation with terms and opeation parsers.
-- Builds up an expression tree, by chaining, according to the operation's respective priority.
pOperation :: Parser Expr -> Parser (Expr -> Expr -> Prim Expr) -> Parser Expr
pOperation sub op = do
  lhs <- sub
  fold lhs
  where
    fold lhs = (do
        f <- op
        rhs <- sub
        fold (Op (f lhs rhs))) <|> pure lhs

-- | Parses an arithmetic expression from a string provided.
parseExpr :: String -> Except ParseError Expr
parseExpr = runP (pWs *> pExpr <* pWs <* pEof)
