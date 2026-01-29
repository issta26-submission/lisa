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
//<ID> 448
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "foo|bar|hello";
    const char text_buf[] = "hello foo bar";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_string_t matches[2];
    cre2_string_t text_sp;
    int added_ret = 0;
    int one_line_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char *pat_back = NULL;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&text_sp, 0, sizeof(text_sp));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    one_line_flag = cre2_opt_one_line(opt);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    added_ret = cre2_set_add_simple(set, set_pattern);
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate
    pat_back = cre2_pattern(re);

    // step 5: Validate
    computed_check = added_ret + one_line_flag + (int)enc + pattern_len + (int)(pat_back != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}