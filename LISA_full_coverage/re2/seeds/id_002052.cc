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
//<ID> 2052
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<word>[A-Za-z]+)-(?P<num>[0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "example-12345";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t *opt = NULL;
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int matches[8];
    size_t match_count = 0;
    int one_line_before = 0;
    int one_line_after = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char *pat_from_re = NULL;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    one_line_before = cre2_opt_one_line(opt);
    cre2_opt_set_one_line(opt, 1);
    one_line_after = cre2_opt_one_line(opt);
    enc = cre2_opt_encoding(opt);

    // step 3: Operate
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    memset(matches, 0, sizeof(matches));
    match_count = cre2_set_match(set, text, text_len, matches, 8);
    pat_from_re = cre2_pattern(re);

    // step 4: Validate
    checksum = (long)pattern_len + (long)match_count + (long)matches[0] + (long)one_line_before + (long)one_line_after + (long)enc + (long)(pat_from_re != NULL) + (long)(iter != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
    // API sequence test completed successfully
}