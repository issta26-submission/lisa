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
//<ID> 1644
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
    const char input_text[] = "prefix foobar middle foo suffix";
    const size_t input_text_len = (size_t)(sizeof(input_text) - 1);
    char text_and_target_buf[128];
    const size_t text_and_target_buf_len = (sizeof(text_and_target_buf) / sizeof(text_and_target_buf[0]));
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    int match[8];
    size_t nmatched = 0;
    const char * compiled_pat = NULL;
    int rc_extract = 0;
    int rc_replace = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate
    memcpy(text_and_target_buf, input_text, input_text_len);
    text_and_target_buf[input_text_len < text_and_target_buf_len ? input_text_len : text_and_target_buf_len - 1] = '\0';
    text_str.data = input_text;
    text_str.length = (int)input_text_len;
    rewrite_str.data = "$1";            // use first capture as rewrite pattern
    rewrite_str.length = (int)(sizeof("$1") - 1);
    target_str.data = text_and_target_buf; // target buffer for extraction (may be used by API)
    target_str.length = (int)(text_and_target_buf_len - 1);
    rc_extract = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);
    rc_replace = cre2_global_replace_re(re, &target_str, &rewrite_str);
    memset(match, -1, sizeof(match));
    nmatched = cre2_set_match(set, target_str.data, (size_t)target_str.length, match, (size_t)(sizeof(match) / sizeof(match[0])));
    compiled_pat = cre2_pattern(re);

    // step 5: Validate
    checksum = (long)pattern_len + (long)rc_extract + (long)rc_replace + (long)nmatched + (long)match[0]
               + (long)(compiled_pat != NULL) + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}