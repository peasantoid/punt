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

REPORT_MODULE("parse",
              "eval");

MFUNC_PROTO(parse) {
  check_argc("parse", 1, args);
  check_argt("parse", args, P_STR, 0);

  return make_atom(P_BLOCK, "", (void *)parse_tokens(
        tokenize_str((char *)args->value)));
}

MFUNC_PROTO(eval) {
  check_argc("eval", 1, args);
  check_argt("eval", args, P_BLOCK, 0);

  return run_exp((p_atom *)args->value, vars);
}
