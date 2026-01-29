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
//<ID> 2083
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^([A-Za-z]+)([0-9]+)$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Bob007";
    int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t input = { text_buf, text_len };
    cre2_string_t matches[3];
    memset(matches, 0, sizeof(matches));
    long checksum = 0;

    // step 2: Setup (create options and set)
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    int ver_rev = cre2_version_interface_revision();

    // step 3: Configure (add simple pattern to set and compile)
    int add_result = cre2_set_add_simple(set, pattern);
    int compile_result = cre2_set_compile(set);

    // step 4: Core operations (build regexp from pattern and perform full match using regexp API)
    re = cre2_new(pattern, pattern_len, opt);
    int full_match_result = cre2_full_match_re(re, &input, matches, 3);

    // step 5: Validate (combine results so values flow through)
    checksum = (long)pattern_len + (long)text_len + (long)add_result + (long)compile_result + (long)full_match_result + (long)ver_rev + (long)matches[0].length + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup (delete regexp, set and options)
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}