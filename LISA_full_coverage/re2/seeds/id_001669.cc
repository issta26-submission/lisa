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
//<ID> 1669
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(cd)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "abcd abc ab";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char match_buf[64];
    char text_and_target_buf[128];
    char rewrite_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    const char * compiled_pat = NULL;
    cre2_string_t match_str;
    cre2_string_t text_and_target_str;
    cre2_string_t rewrite_str;
    int easy_match_rc = 0;
    int replace_rc = 0;
    int cs_flag = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cs_flag = cre2_opt_case_sensitive(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    /* cre2_set_new expects a cre2_anchor_t for the second argument; cast 0 to that type */
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);

    // step 4: Operate
    compiled_pat = cre2_pattern(re);
    memset(match_buf, 0, sizeof(match_buf));
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memcpy(text_and_target_buf, source_text, (size_t)source_text_len);
    memcpy(rewrite_buf, "$1", 2);

    match_str.data = match_buf;
    match_str.length = 0;

    text_and_target_str.data = text_and_target_buf;
    text_and_target_str.length = source_text_len;

    rewrite_str.data = rewrite_buf;
    rewrite_str.length = 2;

    easy_match_rc = cre2_easy_match(pattern, pattern_len, source_text, source_text_len, &match_str, 1);
    replace_rc = cre2_replace_re(re, &text_and_target_str, &rewrite_str);

    // step 5: Validate
    checksum = (long)pattern_len + (long)easy_match_rc + (long)replace_rc + (long)cs_flag + (long)(compiled_pat != NULL)
               + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)match_str.length + (long)text_and_target_str.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}