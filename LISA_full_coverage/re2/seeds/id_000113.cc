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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello) (world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "greeting: hello world!";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_template[] = "$2,$1";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    int nmatch = 3;
    int match_res = 0;
    int check_res = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    match_res = cre2_match((const cre2_regexp_t *)re, text, text_len, 0, text_len, CRE2_UNANCHORED, &matches[0], nmatch);
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 5: Validate
    (void)match_res;
    (void)check_res;
    (void)matches;
    (void)rewrite;
    (void)errmsg;
    (void)text;
    (void)text_len;
    (void)pattern_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}