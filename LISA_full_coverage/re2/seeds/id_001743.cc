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
//<ID> 1743
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
    const char source_text[] = "foo and foobar and foo";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    cre2_string_t match_arr[3];
    int easy_match_res = 0;
    int set_add_res = 0;
    size_t set_match_count = 0;
    int match_indices[8];
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_res = cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 3: Configure
    match_arr[0].data = NULL; match_arr[0].length = 0;
    match_arr[1].data = NULL; match_arr[1].length = 0;
    match_arr[2].data = NULL; match_arr[2].length = 0;
    memset(match_indices, 0, sizeof(match_indices));

    // step 4: Operate
    easy_match_res = cre2_easy_match(pattern, pattern_len, source_text, source_text_len, match_arr, 3);
    set_match_count = cre2_set_match(set, source_text, (size_t)source_text_len, match_indices, (size_t)(sizeof(match_indices)/sizeof(match_indices[0])));

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)easy_match_res + (long)set_add_res + (long)set_match_count + (long)match_arr[0].length + (long)match_indices[0] + (long)(re != NULL) + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}