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
//<ID> 1646
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
    const char text[] = "xx foobar yy foo zz";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    const char * compiled_pat = NULL;
    int match[8];
    size_t nmatched = 0;
    int extract_ret = 0;
    int greplace_ret = 0;
    long checksum = 0;
    char extract_buf[128];
    char rewrite_buf[64];
    char replace_buf[128];

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate
    compiled_pat = cre2_pattern(re);
    memset(match, -1, sizeof(match));
    nmatched = cre2_set_match(set, text, text_len, match, (size_t)(sizeof(match) / sizeof(match[0])));

    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    text_str.data = text;
    text_str.length = (int)text_len;
    rewrite_buf[0] = '\\';
    rewrite_buf[1] = '1';
    rewrite_buf[2] = '-';
    rewrite_buf[3] = '\\';
    rewrite_buf[4] = '2';
    rewrite_buf[5] = '\0';
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = (int)(sizeof(rewrite_buf) - 1);
    target_str.data = extract_buf;
    target_str.length = 0;
    extract_ret = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);

    cre2_string_t text_and_target;
    cre2_string_t replace_rewrite;
    text_and_target.data = replace_buf;
    memcpy(replace_buf, text, text_len);
    replace_buf[text_len] = '\0';
    text_and_target.length = (int)text_len;
    replace_rewrite.data = rewrite_buf;
    replace_rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    greplace_ret = cre2_global_replace_re(re, &text_and_target, &replace_rewrite);

    // step 5: Validate
    checksum = (long)pattern_len + (long)(compiled_pat != NULL) + (long)nmatched + (long)match[0] + (long)match[1]
               + (long)extract_ret + (long)greplace_ret + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}