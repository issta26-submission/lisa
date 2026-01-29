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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "prefix foobar suffix";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_template[] = "$1-$2";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    cre2_string_t matches[4];
    cre2_string_t quoted;
    cre2_string_t original;
    cre2_string_t rewrite;
    cre2_string_t text_and_target;
    cre2_string_t errmsg;
    int easy_res = 0;
    int quote_res = 0;
    int check_res = 0;
    int replace_res = 0;
    int combined = 0;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(&quoted, 0, sizeof(quoted));
    memset(&original, 0, sizeof(original));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&errmsg, 0, sizeof(errmsg));
    original.data = rewrite_template;
    original.length = (int)(sizeof(rewrite_template) - 1);
    text_and_target.data = text;
    text_and_target.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &matches[0], 3);
    quote_res = cre2_quote_meta(&quoted, &original);
    /* choose the quoted rewrite as the rewrite template for subsequent operations */
    rewrite = quoted;
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (use results to satisfy data-flow; no branching)
    combined = easy_res + quote_res + check_res + replace_res + (int)enc;
    (void)combined;
    (void)matches;
    (void)original;
    (void)text_and_target;
    (void)errmsg;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}