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
//<ID> 2055
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>[A-Za-z]+)(?P<num>[0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "Alice123";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t *opt = NULL;
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int matches[8];
    size_t match_count = 0;
    const char *got_pattern = NULL;
    int one_line_before = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    one_line_before = cre2_opt_one_line(opt);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations - create set and regexp, add pattern and compile
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Use named-groups iterator, retrieve pattern and perform matching
    iter = cre2_named_groups_iter_new(re);
    got_pattern = cre2_pattern(re);
    memset(matches, 0, sizeof(matches));
    match_count = cre2_set_match(set, text, text_len, matches, 8);

    // step 5: Validate (simple checksum to ensure values flow through)
    checksum = (long)pattern_len + (long)one_line_before + (long)enc + (long)match_count + (long)matches[0] + (long)(got_pattern != NULL) + (long)(iter != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}