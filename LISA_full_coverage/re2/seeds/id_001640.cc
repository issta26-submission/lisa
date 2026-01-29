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
//<ID> 1640
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)|(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "XXfooYYbarZZfoo";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * rex = NULL;
    cre2_set * set = NULL;
    const char * compiled_pat = NULL;
    int match[8];
    size_t nmatched = 0;
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    char rewrite_buf[] = "[$0]";
    char target_buf[64];
    int extract_res = 0;
    int replace_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);

    // step 3: Configure
    rex = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate
    compiled_pat = cre2_pattern(rex);
    memset(match, -1, sizeof(match));
    nmatched = cre2_set_match(set, text, text_len, match, (size_t)(sizeof(match) / sizeof(match[0])));
    text_str.data = text;
    text_str.length = (int)text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = (int)(sizeof(rewrite_buf) - 1);
    target_str.data = target_buf;
    target_str.length = 0;
    extract_res = cre2_extract_re(rex, &text_str, &rewrite_str, &target_str);
    replace_res = cre2_global_replace_re(rex, &text_str, &rewrite_str);

    // step 5: Validate
    checksum = (long)pattern_len + (long)nmatched + (long)match[0] + (long)match[1] + (long)match[2]
               + (long)(compiled_pat != NULL) + (long)extract_res + (long)replace_res + (long)(opt != NULL)
               + (long)(rex != NULL) + (long)(set != NULL) + (long)(target_str.length);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(rex);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}