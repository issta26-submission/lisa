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
//<ID> 73
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)?(c)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xxabcxx";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t matches[64];
    cre2_string_t partial_match;
    int perl_flag = 0;
    int groups = 0;
    int nmatch = 0;
    int easy_res = 0;
    int partial_res = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(matches, 0, sizeof(matches));
    memset(&partial_match, 0, sizeof(partial_match));
    text_str.data = text;
    text_str.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_perl_classes(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    groups = cre2_num_capturing_groups(re);
    nmatch = groups + 1;
    if (nmatch < 1) nmatch = 1; /* single straight-line adjustment without altering control flow meaningfully */
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, matches, nmatch);
    partial_res = cre2_partial_match(pattern, &text_str, &partial_match, nmatch);

    // step 5: Validate
    (void)perl_flag;
    (void)groups;
    (void)nmatch;
    (void)easy_res;
    (void)partial_res;
    (void)matches;
    (void)partial_match;
    (void)text_str;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}