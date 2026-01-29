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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char original_pattern[] = "(foo)(bar)?";
    const int original_pattern_len = (int)(sizeof(original_pattern) - 1);
    char text_and_target_buf[64] = "prefix foobar suffix";
    const int text_len = (int)(sizeof("prefix foobar suffix") - 1);
    char quoted_buf[128];
    char errmsg_buf[128];
    const char rewrite_template[] = "$1-$2";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[4];
    cre2_string_t original;
    cre2_string_t quoted;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    cre2_encoding_t enc;
    int easy_res = 0;
    int quoted_len = 0;
    int check_res = 0;
    int replace_res = 0;
    int nmatch = 3;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(quoted_buf, 0, sizeof(quoted_buf));
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    original.data = original_pattern;
    original.length = original_pattern_len;
    quoted.data = quoted_buf;
    quoted.length = (int)sizeof(quoted_buf);
    text_and_target.data = text_and_target_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    errmsg.data = errmsg_buf;
    errmsg.length = (int)sizeof(errmsg_buf);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    easy_res = cre2_easy_match(original_pattern, original_pattern_len, text_and_target.data, text_len, &matches[0], nmatch);
    quoted_len = cre2_quote_meta(&quoted, &original);
    re = cre2_new(quoted.data, quoted_len, opt);
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (use results to satisfy data-flow; no branching)
    (void)easy_res;
    (void)quoted_len;
    (void)check_res;
    (void)replace_res;
    (void)enc;
    (void)matches;
    (void)errmsg;
    (void)text_and_target;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}