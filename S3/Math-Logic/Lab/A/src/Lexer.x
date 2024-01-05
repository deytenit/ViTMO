{
module Lexer where
}

%wrapper "basic"

$digit = 0-9
$alpha = [A-Z]

tokens :-
  $white                     ;
  "->"                       { \s -> TImpl }
  [\|\&\!\(\)]             { \s -> TSym (head s)}
  $alpha [$alpha $digit \']* { \s -> TVar s}

{
data Token = TImpl | TSym Char | TVar String deriving (Eq, Show)
}
