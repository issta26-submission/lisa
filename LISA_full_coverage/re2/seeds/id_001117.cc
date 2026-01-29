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
//<ID> 1117
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "abc([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "abc123 xyz abc456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t matches[3];
    cre2_string_t text_obj;
    int nmatch = 0;
    int compile_ret = 0;
    int ngroups = 0;
    int match_ret = 0;
    long computed_check = 0;

    // step 2: Setup / Configure options
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Create and compile a set with the options
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    (void)cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);

    // step 4: Create regexp and prepare match targets
    re = cre2_new(pattern, pattern_len, opt);
    ngroups = cre2_num_capturing_groups(re);
    memset(matches, 0, sizeof(matches));
    text_obj.data = text_buf;
    text_obj.length = text_len;
    nmatch = (int)(sizeof(matches) / sizeof(matches[0]));

    // step 5: Perform a match using the compiled regexp
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);

    // step 6: Validate / Cleanup
    computed_check = (long)compile_ret + (long)ngroups + (long)match_ret + (long)matches[0].length + (long)pattern_len + (long)text_len;
    (void)computed_check;
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}