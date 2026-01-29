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
//<ID> 835
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<g>foo)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foobar yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int case_flag = 1;
    int match_slots[4];
    size_t matched_count = 0;
    const char * found_name = NULL;
    int found_index = -1;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, case_flag);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Operate - create and populate set, then match
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_add_simple(set, "foo");
    cre2_set_compile(set);
    memset(match_slots, 0, sizeof(match_slots));
    matched_count = cre2_set_match(set, text_buf, (size_t)text_len, match_slots, 4);

    // step 4: Operate - create regexp and iterate named groups once
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(iter, &found_name, &found_index);
    cre2_named_groups_iter_delete(iter);

    // step 5: Validate
    computed_check = (long)matched_count + (long)enc + (long)match_slots[0] + (long)(found_name ? found_name[0] : 0) + (long)found_index + (long)case_flag;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}