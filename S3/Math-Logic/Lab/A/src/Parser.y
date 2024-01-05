{
module Parser where

import Lexer
}

%name parser
%tokentype { Token }
%error { parseError }

%token
  '>'         { TImpl }
  '|'         { TSym '|' }
  '&'         { TSym '&' }
  '!'         { TSym '!' }
  '('         { TSym '(' }
  ')'         { TSym ')' }
  var         { TVar $$ }

%%

Expression:
  Disjunction                   { $1 }
  | Disjunction '>' Expression  { Impl $1 $3 }

Disjunction:
  Conjunction                   { $1 }
  | Disjunction '|' Conjunction { Or $1 $3 }

Conjunction:
  Negation                      { $1 }
  | Conjunction '&' Negation    { And $1 $3 }

Negation:
  '!' Negation                  { Not $2 }
  | var                         { Var $1 }
  | '(' Expression ')'          { $2 }

{
parseError :: [Token] -> a
parseError _ = error "Parse error"

data Expression = Impl { getT1 :: Expression, getT2 :: Expression }
                | Or   { getT1 :: Expression, getT2 :: Expression }
                | And  { getT1 :: Expression, getT2 :: Expression }
                | Not  Expression
                | Var  String
                deriving (Eq, Ord)
}
