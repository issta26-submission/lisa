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
//<ID> 123
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
    const char input_text[] = "prefix foobar suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const char rewrite_template[] = "$1-$2";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t original;
    cre2_string_t quoted;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    cre2_string_t matches[3];
    cre2_string_t text_and_target;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int quote_res = 0;
    int easy_res = 0;
    int check_res = 0;
    int replace_res = 0;
    int nmatch = 3;

    // step 2: Setup
    memset(&original, 0, sizeof(original));
    memset(&quoted, 0, sizeof(quoted));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));
    memset(&matches, 0, sizeof(matches));
    memset(&text_and_target, 0, sizeof(text_and_target));
    original.data = pattern;
    original.length = pattern_len;
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    text_and_target.data = input_text;
    text_and_target.length = input_text_len;
    opt = cre2_opt_new();

    // step 3: Configure / Query
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    quote_res = cre2_quote_meta(&quoted, &original);
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nmatch);
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (use results to satisfy data-flow; no branching)
    (void)enc;
    (void)quote_res;
    (void)easy_res;
    (void)check_res;
    (void)replace_res;
    (void)matches;
    (void)quoted;
    (void)errmsg;
    (void)text_and_target;
    (void)original;
    (void)rewrite;
    (void)nmatch;
    (void)pattern_len;
    (void)input_text_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}