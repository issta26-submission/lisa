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
//<ID> 1641
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
    char text_and_target_buf[] = "zzfoobaz foobar foo";
    const char * text = text_and_target_buf;
    const size_t text_len = (size_t)(sizeof(text_and_target_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    const char * compiled_pat = NULL;
    int match[8];
    cre2_string_t text_s;
    cre2_string_t rewrite_s;
    cre2_string_t target_s;
    cre2_string_t text_and_target_s;
    char rewrite_buf[] = "[$1]";
    char target_buf[128];
    size_t nmatched = 0;
    int rc_extract = 0;
    int rc_replace = 0;
    long checksum = 0;

    // step 2: Setup
    memset(match, -1, sizeof(match));
    memset(target_buf, 0, sizeof(target_buf));
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Core operations
    compiled_pat = cre2_pattern(re);
    text_s.data = text;
    text_s.length = (int)text_len;
    rewrite_s.data = rewrite_buf;
    rewrite_s.length = (int)(sizeof(rewrite_buf) - 1);
    target_s.data = target_buf;
    target_s.length = 0;
    text_and_target_s.data = text_and_target_buf;
    text_and_target_s.length = (int)text_len;
    nmatched = cre2_set_match(set, text, text_len, match, (size_t)(sizeof(match) / sizeof(match[0])));
    rc_extract = cre2_extract_re(re, &text_s, &rewrite_s, &target_s);
    rc_replace = cre2_global_replace_re(re, &text_and_target_s, &rewrite_s);

    // step 5: Validate
    checksum = (long)pattern_len + (long)nmatched + (long)match[0] + (long)match[1] + (long)match[2]
               + (long)rc_extract + (long)rc_replace + (long)(compiled_pat != NULL)
               + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL)
               + (long)text_s.length + (long)text_and_target_s.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}