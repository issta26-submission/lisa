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
//<ID> 888
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foobar yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * pattern_from_re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t matches[4];
    int find_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_dot_nl(opt, 0);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);
    memset(matches, 0, sizeof(matches));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    find_res = cre2_find_and_consume(pattern_from_re, &text_and_target, matches, 4);

    // step 4: Validate
    computed_check = (long)pattern_len + (long)text_and_target.length + (long)matches[0].length + (long)find_res + (long)strlen(pattern_from_re);
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_named_groups_iter_delete(iter);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}