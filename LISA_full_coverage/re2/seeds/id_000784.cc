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
//<ID> 784
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "ab(c)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char src_text[] = "xx abc yy ab zz";
    const int src_text_len = (int)(sizeof(src_text) - 1);
    char working_text[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t match;
    int rc_easy_match = 0;
    int num_groups = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(working_text, 0, sizeof(working_text));
    memcpy(working_text, src_text, (size_t)src_text_len);
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    match.data = NULL;
    match.length = 0;
    rc_easy_match = cre2_easy_match(pattern, pattern_len, working_text, src_text_len, &match, 2);

    // step 4: Validate
    num_groups = cre2_num_capturing_groups(re);
    computed_check = rc_easy_match + match.length + num_groups + (int)working_text[0] + pattern_len;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}