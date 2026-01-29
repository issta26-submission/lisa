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
//<ID> 994
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "alice|bob";
    const int set_pattern_len = (int)(sizeof(set_pattern) - 1);
    char text_buf[] = "alice123 bob456 alice789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int set_add_res = 0;
    int set_compile_res = 0;
    size_t set_matches = 0;
    int matches[16];
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Operate - compile regexp and create named-groups iterator
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate - create a set, add simple pattern, compile and match against text
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_res = cre2_set_add_simple(set, set_pattern);
    set_compile_res = cre2_set_compile(set);
    memset(matches, 0, sizeof(matches));
    set_matches = cre2_set_match(set, text_buf, text_len, matches, (int)(sizeof(matches)/sizeof(matches[0])));

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)enc + (long)set_add_res + (long)set_compile_res + (long)set_matches + (long)pattern_len + (long)text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}