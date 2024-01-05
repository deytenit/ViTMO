module Main where

import Lexer
import Parser

-- Grammar

instance Show Expression where
  show (Impl disjunction expression) = "(->," ++ (show disjunction) ++ "," ++ (show expression) ++ ")"
  show (Or disjunction conjunction) = "(|," ++ (show disjunction) ++ "," ++ (show conjunction) ++ ")"
  show (And conjunction negation) = "(&," ++ (show conjunction) ++ "," ++ (show negation) ++ ")"
  show (Not negation) = "!" ++ (show negation)
  show (Var string) = string


-- Main

main :: IO ()
main = do
  line <- Prelude.getLine
  putStrLn (show (parser (alexScanTokens line)))

