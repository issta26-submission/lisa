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
//<ID> 1742
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
    const char source_text[] = "foo bar foobar";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t match_arr[4];
    cre2_set * set = NULL;
    int set_matches_buf[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
    size_t set_match_res = 0;
    int easy_match_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Configure buffers and match array
    match_arr[0].data = NULL; match_arr[0].length = 0;
    match_arr[1].data = NULL; match_arr[1].length = 0;
    match_arr[2].data = NULL; match_arr[2].length = 0;
    match_arr[3].data = NULL; match_arr[3].length = 0;

    // step 4: Operate - run easy match and set match
    easy_match_res = cre2_easy_match(pattern, pattern_len, source_text, source_text_len, match_arr, 4);
    /* cast 0 to cre2_anchor_t to match the function signature */
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, "(foo)");
    cre2_set_add_simple(set, "(bar)");
    cre2_set_compile(set);
    set_match_res = cre2_set_match(set, source_text, (size_t)source_text_len, set_matches_buf, 8);

    // step 5: Validate (simple checksum to make use of results)
    checksum = (long)pattern_len + (long)source_text_len + (long)easy_match_res + (long)set_match_res + (long)(match_arr[0].length) + (long)set_matches_buf[0] + (long)(re != NULL) + (long)(opt != NULL) + (long)(set != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}