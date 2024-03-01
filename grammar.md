# Grammar

### Pre-Notes

 * "?" is the nil-check operator. If the expression gets nil, an error is thrown.

 * All functions return nil by default.

 * The `main` function is required and returns nil.

### BNF Rules
```bnf
; basic tokens
whitespace ::= (SP | TAB | CR | LF)+
line-comment ::= "#" (non-newline) LF
keyword ::= (ALPHA)+
identifier ::= (ALPHA | "_")+
nil ::= "$N"
boolean ::= "$T" | "$F"
numeric ::= (DIGIT)+ "." (DIGIT)+
string ::= "\"" (non-double-quotes) "\""
list ::= "[" (literal-expr ",")* "]"
object ::= identifier "{" (literal-expr ",")* "}"

; basic expressions
call-expr ::= access-expr "(" (element-expr ",")* ")"
element-expr ::= nil | boolean | numeric | string | list | object | (expr)
access-expr ::= identifier "[" ((string | numeric) ",")* "]"
unary-expr ::= ("-" | "?")* (access-expr | element-expr)
factor-expr ::= unary-expr ("*" || "/" unary-expr)*
term-expr ::= factor-expr ("+" || "-" factor-expr)*
comparison-expr ::= term-expr ("==" || "!=" || "<=" || ">=" || "<" || ">" term-expr)*
conditional-expr ::= comparison-expr ("&&" | "||" comparison-expr)*
expr ::= conditional-expr

; basic stmts
use-stmt ::= "use" identifier
var-decl ::= ("let" | "mut") identifier "=" expr
param-decl ::= ("val" | "ref") identifier
func-decl ::= "fun" identifier "(" (param-decl ",")* ")" block
field-decl ::= "field" identifier
object-decl ::= "object" identifier (field-decl)* "end"
assign-stmt ::= access-expr "=" expr
return-stmt ::= "ret" expr
if-stmt ::= "if" conditional-expr block (else-stmt){0,1}
else-stmt ::= "else" block
while-stmt ::= "while" conditional-expr block
expr-stmt ::= call-expr
sub-stmt ::= assign-stmt | expr-stmt | var-stmt | if-stmt | while-stmt | return-stmt
stmt ::= use-decl | var-stmt | func-decl | object-decl
block ::= (sub-stmt)+ "end"
program ::= (stmt)*
```
