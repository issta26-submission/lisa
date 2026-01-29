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
//<ID> 1643
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
    const char original_text[] = "foo foobar bar foofoobar";
    const size_t original_text_len = (size_t)(sizeof(original_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    const char * compiled_pat = NULL;
    int match[8];
    size_t nmatched = 0;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t extract_rewrite;
    cre2_string_t extract_target;
    int replace_result = 0;
    int extract_result = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate
    compiled_pat = cre2_pattern(re);
    memset(match, -1, sizeof(match));
    nmatched = cre2_set_match(set, original_text, original_text_len, match, (size_t)(sizeof(match) / sizeof(match[0])));
    text_and_target.data = original_text;
    text_and_target.length = (int)original_text_len;
    rewrite.data = "[$1]";
    rewrite.length = 4; // including brackets and $1
    replace_result = cre2_global_replace_re(re, &text_and_target, &rewrite);
    extract_rewrite.data = "$2";
    extract_rewrite.length = 2;
    extract_target.data = NULL;
    extract_target.length = 0;
    extract_result = cre2_extract_re(re, &text_and_target, &extract_rewrite, &extract_target);

    // step 5: Validate
    checksum = (long)pattern_len + (long)nmatched + (long)match[0] + (long)match[1] + (long)match[2]
               + (long)(compiled_pat != NULL) + (long)replace_result + (long)extract_result + (long)(opt != NULL)
               + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}