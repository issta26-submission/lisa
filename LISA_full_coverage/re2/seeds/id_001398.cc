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
//<ID> 1398
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char set_pattern[] = "foo|bar|(baz)";
    const int set_pattern_len = (int)(sizeof(set_pattern) - 1);
    const char re_pattern[] = "(item)(\\d+)";
    const int re_pattern_len = (int)(sizeof(re_pattern) - 1);
    const char replace_pattern[] = "(\\d+)";
    const int replace_pattern_len = (int)(sizeof(replace_pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    char errbuf[128];
    size_t errbuf_len = sizeof(errbuf);
    int add_ret = 0;
    int replace_ret = 0;
    int global_replace_ret = 0;
    int num_groups = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&checksum, 0, sizeof(checksum));
    memset(errbuf, 0, errbuf_len);
    opt = cre2_opt_new();

    // step 3: Configure / Operate
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_ret = cre2_set_add(set, set_pattern, (size_t)set_pattern_len, errbuf, errbuf_len);
    re = cre2_new(re_pattern, re_pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);

    const char textbuf[] = "item123 and item456";
    const int textbuf_len = (int)(sizeof(textbuf) - 1);
    const char rewrite_buf[] = "[$0]";
    const int rewrite_buf_len = (int)(sizeof(rewrite_buf) - 1);
    text_and_target.data = textbuf;
    text_and_target.length = textbuf_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_buf_len;

    replace_ret = cre2_replace(replace_pattern, &text_and_target, &rewrite);
    global_replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate / lightweight checksum
    checksum = (long)set_pattern_len + (long)re_pattern_len + (long)replace_pattern_len
               + (long)num_groups + (long)add_ret + (long)replace_ret + (long)global_replace_ret
               + (long)(set != NULL) + (long)(re != NULL) + (long)text_and_target.length;
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}