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
//<ID> 788
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foo bar yy foo zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t match;
    int rc_easy_match = 0;
    int rc_set_add = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();
    if (opt) cre2_opt_set_case_sensitive(opt, 1);

    /* cre2_set_new expects a cre2_anchor_t; use a cast of 0 to that type to
       preserve original logic while matching the function signature. */
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Operate
    if (set) rc_set_add = cre2_set_add_simple(set, pattern);
    re = cre2_new(pattern, pattern_len, opt);
    match.data = NULL;
    match.length = 0;
    rc_easy_match = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 1);

    // step 4: Validate
    computed_check = rc_easy_match + rc_set_add + match.length + (opt ? cre2_opt_case_sensitive(opt) : 0);
    (void)computed_check;

    // step 5: Cleanup
    if (re) cre2_delete(re);
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}