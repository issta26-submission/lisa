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
//<ID> 687
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foobar yy foo zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    struct cre2_set * set = NULL;
    const char * got_pattern = NULL;
    int add_rc = 0;
    int compile_rc = 0;
    int nmatches = 0;
    int match_buf[4];
    int computed_check = 0;

    // step 2: Setup
    memset(match_buf, -1, sizeof(match_buf));
    opt = cre2_opt_new();
    cre2_opt_set_never_nl(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 4: Operate
    add_rc = cre2_set_add_simple(set, pattern);
    compile_rc = cre2_set_compile(set);
    nmatches = (int)cre2_set_match(set, text_buf, text_len, match_buf, 4);
    got_pattern = cre2_pattern(re);

    // step 5: Validate
    computed_check = add_rc + compile_rc + nmatches + (int)match_buf[0] + (int)(got_pattern ? got_pattern[0] : 0);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}