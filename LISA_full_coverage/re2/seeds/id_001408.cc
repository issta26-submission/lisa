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
//<ID> 1408
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char set_pattern[] = "alpha";
    const char re_pattern[] = "(alpha)(beta)?";
    const char text[] = "alpha beta gamma";
    const int set_pattern_len = (int)(sizeof(set_pattern) - 1);
    const int re_pattern_len = (int)(sizeof(re_pattern) - 1);
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    int ret_add_simple = 0;
    int ret_easy = 0;
    int num_groups = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 4: Core operations
    ret_add_simple = cre2_set_add_simple(set, set_pattern);
    re = cre2_new(re_pattern, re_pattern_len, opt);
    ret_easy = cre2_easy_match(re_pattern, re_pattern_len, text, text_len, &match, 3);
    num_groups = (re != NULL) ? cre2_num_capturing_groups(re) : 0;

    // step 5: Validate
    checksum = (long)ret_add_simple + (long)ret_easy + (long)match.length + (long)num_groups + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}