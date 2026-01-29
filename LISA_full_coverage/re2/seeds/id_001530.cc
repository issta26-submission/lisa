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
//<ID> 1530
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text;
    cre2_string_t rewrite;
    cre2_string_t target;
    cre2_string_t match;
    char textbuf[] = "prefix foo middle";
    char rewritebuf[] = "replacement";
    char targetbuf[64];
    int add_ret = 0;
    int match_ret = 0;
    int extract_ret = 0;
    int dotnl_flag = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&text, 0, sizeof(text));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&target, 0, sizeof(target));
    memset(&match, 0, sizeof(match));
    memset(targetbuf, 0, sizeof(targetbuf));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_dot_nl(opt, 1);
    dotnl_flag = cre2_opt_dot_nl(opt);

    // step 4: Core operations
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret = cre2_set_add_simple(set, pattern);
    re = cre2_new(pattern, pattern_len, opt);
    text.data = textbuf;
    text.length = (int)(sizeof(textbuf) - 1);
    rewrite.data = rewritebuf;
    rewrite.length = (int)(sizeof(rewritebuf) - 1);
    target.data = targetbuf;
    target.length = (int)(sizeof(targetbuf) - 1);
    match.data = NULL;
    match.length = 0;
    match_ret = cre2_match((const cre2_regexp_t *)re, text.data, text.length, 0, text.length, CRE2_UNANCHORED, &match, 1);
    extract_ret = cre2_extract_re(re, &text, &rewrite, &target);

    // step 5: Validate
    checksum = (long)pattern_len + (long)add_ret + (long)match_ret + (long)extract_ret + (long)dotnl_flag
               + (long)(set != NULL) + (long)(re != NULL) + (long)text.length + (long)rewrite.length + (long)target.length + (long)match.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}