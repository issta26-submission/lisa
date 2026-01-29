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
//<ID> 1745
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
    const char subject[] = "foo bar foobar and barfoo";
    const int subject_len = (int)(sizeof(subject) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t match_arr[4];
    const char * found_name = NULL;
    int found_index = -1;
    bool iter_next_res = false;
    cre2_set * set = NULL;
    int set_matches[8];
    size_t set_match_count = 0;
    int easy_match_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure
    match_arr[0].data = NULL; match_arr[0].length = 0;
    match_arr[1].data = NULL; match_arr[1].length = 0;
    match_arr[2].data = NULL; match_arr[2].length = 0;
    match_arr[3].data = NULL; match_arr[3].length = 0;
    memset(set_matches, 0, sizeof(set_matches));
    cre2_set_add_simple(set, "foo");
    cre2_set_add_simple(set, "bar");
    cre2_set_compile(set);

    // step 4: Operate
    easy_match_res = cre2_easy_match(pattern, pattern_len, subject, subject_len, match_arr, 4);
    iter_next_res = cre2_named_groups_iter_next(iter, &found_name, &found_index);
    set_match_count = cre2_set_match(set, subject, (size_t)subject_len, set_matches, (size_t)(sizeof(set_matches) / sizeof(set_matches[0])));

    // step 5: Validate
    checksum = (long)pattern_len + (long)subject_len + (long)easy_match_res + (long)iter_next_res + (long)found_index + (long)set_match_count + (long)(re != NULL) + (long)(opt != NULL) + (long)(set != NULL) + (long)(match_arr[0].length);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}