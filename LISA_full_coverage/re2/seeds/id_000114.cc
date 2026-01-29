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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([a-z]+)([0-9]+)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "prefix abc123 suffix";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_template[] = "$2-$1";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UTF8;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    cre2_string_t matches[8];
    int n_captures = 0;
    int nmatch = 0;
    int match_res = 0;
    int check_res = 0;

    // step 2: Setup
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));
    memset(matches, 0, sizeof(matches));
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    cre2_opt_set_encoding(opt, enc);

    // step 4: Operate
    n_captures = cre2_num_capturing_groups(re);
    nmatch = n_captures + 1;
    match_res = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, &matches[0], nmatch);
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 5: Validate
    (void)match_res;
    (void)check_res;
    (void)n_captures;
    (void)nmatch;
    (void)matches;
    (void)rewrite;
    (void)errmsg;
    (void)text;
    (void)text_len;
    (void)pattern;
    (void)pattern_len;
    (void)enc;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}