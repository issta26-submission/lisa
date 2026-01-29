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
//<ID> 1747
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
    cre2_set * set = NULL;
    cre2_string_t match_arr[4];
    int set_match_indices[8];
    size_t set_matched = 0;
    int easy_match_res = 0;
    int set_add_res = 0;
    int set_compile_res = 0;
    long checksum = 0;

    // step 2: Setup options and objects (with safety checks)
    opt = cre2_opt_new();
    if (opt) {
        // leave default options mostly unchanged, but ensure some fields are valid
        cre2_opt_set_log_errors(opt, 0);
    }

    re = cre2_new(pattern, pattern_len, opt);
    if (re) {
        iter = cre2_named_groups_iter_new(re);
    }

    set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        set_add_res = cre2_set_add_simple(set, pattern);
        set_compile_res = cre2_set_compile(set);
    }

    // step 3: Configure temporary containers
    for (int i = 0; i < 4; ++i) {
        match_arr[i].data = NULL;
        match_arr[i].length = 0;
    }
    for (int i = 0; i < 8; ++i) {
        set_match_indices[i] = -1;
    }

    // step 4: Operate / Validate
    // cre2_easy_match compiles the pattern internally, so it's safe to call regardless of 're'
    easy_match_res = cre2_easy_match(pattern, pattern_len, source_text, source_text_len, match_arr, 4);
    if (set) {
        set_matched = cre2_set_match(set, source_text, (size_t)source_text_len, set_match_indices, 8);
    } else {
        set_matched = 0;
    }

    checksum = (long)pattern_len + (long)source_text_len + (long)easy_match_res + (long)set_add_res + (long)set_compile_res + (long)set_matched + (long)set_match_indices[0] + (long)match_arr[0].length + (long)(re != NULL) + (long)(iter != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup (only delete if non-NULL)
    if (iter) cre2_named_groups_iter_delete(iter);
    if (set) cre2_set_delete(set);
    if (re) cre2_delete(re);
    if (opt) cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}