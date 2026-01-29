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
//<ID> 1115
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "Example";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "This is an Example text with Example occurrences.";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[4];
    int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int add_ret = 0;
    int compile_ret = 0;
    int match_ret = 0;
    long checksum = 0;

    // step 2: Setup / Initialize options
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Create and prepare a set with the same options
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret = cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);

    // step 4: Create a regexp with the configured options and prepare match storage
    re = cre2_new(pattern, pattern_len, opt);
    memset(matches, 0, sizeof(matches));

    // step 5: Perform a match using cre2_match and compute a lightweight checksum
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);
    checksum = (long)compile_ret + (long)match_ret + (long)matches[0].length + (long)pattern_len + (long)text_len + (long)add_ret;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}