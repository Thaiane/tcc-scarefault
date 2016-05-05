content_stmt:
  '{' stmt_list '}'
;

stmt_list:
  stmt
| stmt_list stmt
;

stmt:
  comment_stmt { log.message( LogSystem::INFO, "stmt: comment/ GroovyDoc" ); }
| variable_declaration { log.message( LogSystem::SUCCESS, "stmt: variable declaration" ); }
| control_structure_stmt { log.message( LogSystem::WARNING, "stmt: control structure" ); }
| method_stmt { log.message( LogSystem::ERROR, "stmt: method declaration" ); }
;
