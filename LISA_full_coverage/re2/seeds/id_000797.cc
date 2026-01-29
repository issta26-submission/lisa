#include <cre2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 797
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "zz foo bar zz";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int add_rc = 0;
    int compile_rc = 0;
    size_t matched = 0;
    int match_indices[4] = {0, 0, 0, 0};
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int literal_flag = 0;
    int re_nonnull = 0;
    int computed_check = 0;

    // step 2: Setup / Configure
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);
    literal_flag = cre2_opt_literal(opt);
    enc = cre2_opt_encoding(opt);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_rc = cre2_set_add_simple(set, pattern);
    compile_rc = cre2_set_compile(set);
    matched = cre2_set_match(set, text, text_len, match_indices, (size_t)4);

    // step 4: Validate
    re_nonnull = re != NULL;
    computed_check = (int)matched + add_rc + compile_rc + literal_flag + (int)enc + re_nonnull + match_indices[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}