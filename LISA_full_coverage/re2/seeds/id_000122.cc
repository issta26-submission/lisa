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
//<ID> 122
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
    const char initial_text[] = "prefix foobar suffix";
    const int initial_text_len = (int)(sizeof(initial_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    cre2_string_t quoted;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    cre2_string_t text_and_target;
    char quoted_buf[256];
    char rewrite_buf[512];
    char errmsg_buf[128];
    char text_buf[512];
    int easy_res = 0;
    int check_res = 0;
    int replace_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(&quoted, 0, sizeof(quoted));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(quoted_buf, 0, sizeof(quoted_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    memset(text_buf, 0, sizeof(text_buf));
    opt = cre2_opt_new();
    /* prepare an editable copy of the input text for replacement */
    memcpy(text_buf, initial_text, (size_t)initial_text_len);
    text_and_target.data = text_buf;
    text_and_target.length = initial_text_len;

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    /* simple quick match using easy_match to populate capture groups */
    easy_res = cre2_easy_match(pattern, pattern_len, text_and_target.data, text_and_target.length, &matches[0], 3);
    /* prepare quoted version of the first capture (if any) */
    quoted.data = quoted_buf;
    quoted.length = 0;
    /* matches[1] may be zeroed if no capture; passing it is still valid */
    cre2_quote_meta(&quoted, &matches[1]);
    /* build a rewrite template that uses the quoted capture */
    /* use snprintf-style formatting to inject the quoted text into the rewrite template */
    /* rewrite will be "$1-<quoted>-$2" */
    {
        int rlen = snprintf(rewrite_buf, sizeof(rewrite_buf), "$1-%.*s-$2", quoted.length, quoted.data ? quoted.data : "");
        if (rlen < 0) rlen = 0;
        rewrite.data = rewrite_buf;
        rewrite.length = rlen;
    }
    /* compile the regexp with the options and then validate the rewrite template */
    re = cre2_new(pattern, pattern_len, opt);
    errmsg.data = errmsg_buf;
    errmsg.length = 0;
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 5: Validate / Replace
    /* perform a single replace operation using the compiled regexp and the validated rewrite */
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);
    /* use results to satisfy data-flow and avoid unused warnings */
    (void)easy_res;
    (void)check_res;
    (void)replace_res;
    (void)matches;
    (void)quoted;
    (void)errmsg;
    (void)enc;
    (void)initial_text_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}