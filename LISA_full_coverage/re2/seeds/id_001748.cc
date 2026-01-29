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
//<ID> 1748
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
    const char source_text[] = "foobar and foo";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    cre2_string_t easy_matches[3];
    cre2_string_t match_arr[3];
    int set_match_indices[4];
    int easy_res = 0;
    size_t set_matches = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure buffers and structures
    easy_matches[0].data = NULL; easy_matches[0].length = 0;
    easy_matches[1].data = NULL; easy_matches[1].length = 0;
    easy_matches[2].data = NULL; easy_matches[2].length = 0;
    match_arr[0].data = NULL; match_arr[0].length = 0;
    match_arr[1].data = NULL; match_arr[1].length = 0;
    match_arr[2].data = NULL; match_arr[2].length = 0;
    memset(set_match_indices, 0, sizeof(set_match_indices));

    // step 4: Operate (easy match, add to set, compile, set match)
    easy_res = cre2_easy_match(pattern, pattern_len, source_text, source_text_len, easy_matches, 3);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    set_matches = cre2_set_match(set, source_text, (size_t)source_text_len, set_match_indices, (size_t)4);

    // step 5: Validate / compute checksum to use results meaningfully
    checksum = (long)pattern_len + (long)source_text_len + (long)easy_res + (long)set_matches + (long)easy_matches[0].length + (long)easy_matches[1].length + (long)match_arr[0].length + (long)set_match_indices[0] + (long)(re != NULL) + (long)(opt != NULL) + (long)(iter != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}