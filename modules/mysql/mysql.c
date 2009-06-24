/*
 * This file is part of punt.
 *
 * punt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * punt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with punt.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../common.h"
#include <mysql.h>

#define P_MYSQL str_hash("mysql")
#define P_MYSQL_RES str_hash("mysql_res")
#define P_MYSQL_ROW str_hash("mysql_row")

char *errstr;

REPORT_MODULE("mysql_connect",
              "mysql_query",
              "mysql_error",
              "mysql_fetch_row",
              "mysql_field",
              "mysql_escape",
              "mysql_free_result",
              "mysql_close",
              NULL);

MODULE_INIT {
  errstr = "";
}

void seterr(p_atom **vars, MYSQL *conn) {
  errstr = (char *)mysql_error(conn);
  atom_setname(vars, make_atom(P_STR, "__mysql_errstr", (void *)mysql_error(conn)));
}

MFUNC_PROTO(mysql_connect) {
  check_argc("mysql_connect", 5, args);
  check_argt("mysql_connect", args, P_STR, P_STR, P_STR, P_STR, P_NUM, 0);
  
  MYSQL *rval = mysql_init(NULL);
  if(!mysql_real_connect(
        rval,
        (char *)args->value,
        (char *)atom_getindex(args, 1)->value,
        (char *)atom_getindex(args, 2)->value,
        (char *)atom_getindex(args, 3)->value,
        (int)*(p_num *)atom_getindex(args, 4)->value,
        NULL,
        0)) {
    seterr(vars, rval);
    return NIL_ATOM;
  }

  return make_atom(P_MYSQL, "", (void *)rval);
}

MFUNC_PROTO(mysql_query) {
  check_argc("mysql_query", 2, args);
  check_argt("mysql_query", args, P_MYSQL, P_STR, 0);

  MYSQL *conn = (MYSQL *)args->value;
  char *qstr = (char *)atom_getindex(args, 1)->value;

  if(mysql_query(conn, qstr)) {
    seterr(vars, conn);
    return NIL_ATOM;
  }

  MYSQL_RES *res = mysql_use_result(conn);

  return make_atom(P_MYSQL_RES, "", (void *)res);
}

MFUNC_PROTO(mysql_error) {
  check_argc("mysql_error", 0, args);
  return make_atom(P_STR, "", (void *)errstr);
}

MFUNC_PROTO(mysql_fetch_row) {
  check_argc("mysql_fetch_row", 1, args);
  check_argt("mysql_fetch_row", args, P_MYSQL_RES, 0);

  MYSQL_ROW row = mysql_fetch_row((MYSQL_RES *)args->value);
  if(!row) {
    return NIL_ATOM;
  }
  return make_atom(P_MYSQL_ROW, "", (void *)row);
}

MFUNC_PROTO(mysql_field) {
  check_argc("mysql_field", 2, args);
  check_argt("mysql_field", args, P_MYSQL_ROW, P_NUM, 0);

  char *field = ((MYSQL_ROW)args->value)
        [(int)*(p_num *)atom_getindex(args, 1)->value];
  return make_atom(P_STR, "", (void *)field);
}

MFUNC_PROTO(mysql_escape) {
  check_argc("mysql_escape", 2, args);
  check_argt("mysql_escape", args, P_MYSQL, P_STR, 0);

  /*
   * FIXME: basically defeats the purpose of passing the query
   * length by hand
   */
  char *from = (char *)atom_getindex(args, 1)->value;
  int len = strlen(from);
  char *to = (char *)calloc((len * 2) + 1, sizeof(char));
  mysql_real_escape_string((MYSQL *)args->value, to, from, len);

  return make_atom(P_STR, "", (void *)to);
}

MFUNC_PROTO(mysql_free_result) {
  check_argc("mysql_free_result", 1, args);
  check_argt("mysql_free_result", args, P_MYSQL_RES, 0);

  mysql_free_result((MYSQL_RES *)args->value);

  return NIL_ATOM;
}

MFUNC_PROTO(mysql_close) {
  check_argc("mysql_close", 1, args);
  check_argt("mysql_close", args, P_MYSQL, 0);

  mysql_close((MYSQL *)args->value);

  return NIL_ATOM;
}

