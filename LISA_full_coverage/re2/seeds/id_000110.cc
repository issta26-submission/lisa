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
//<ID> 110
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
    char errmsg_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[8];
    cre2_string_t text_str;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    int n_captures = 0;
    int nm = 0;
    int match_res = 0;
    int check_res = 0;
    cre2_encoding_t enc = CRE2_UTF8;
    cre2_anchor_t anchor = CRE2_UNANCHORED;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(&text_str, 0, sizeof(text_str));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    text_str.data = text;
    text_str.length = text_len;
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    errmsg.data = errmsg_buf;
    errmsg.length = (int)(sizeof(errmsg_buf) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, enc);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    n_captures = cre2_num_capturing_groups(re);
    nm = n_captures + 1;
    match_res = cre2_match(re, text, text_len, 0, text_len, anchor, &matches[0], nm);
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 5: Validate
    (void)match_res;
    (void)check_res;
    (void)n_captures;
    (void)nm;
    (void)matches;
    (void)text_str;
    (void)rewrite;
    (void)errmsg;
    (void)anchor;
    (void)enc;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}