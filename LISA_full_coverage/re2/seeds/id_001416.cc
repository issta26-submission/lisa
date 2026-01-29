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
//<ID> 1416
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "hello world example";
    const int text_len = (int)(sizeof(text) - 1);
    const char add_pattern[] = "hello";
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_string_t match[4];
    const int nmatch = 3;
    int ret_set_add = 0;
    int ret_easy = 0;
    long checksum = 0;

    // step 2: Setup
    memset(match, 0, sizeof(match));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Core operations
    ret_set_add = cre2_set_add_simple(set, add_pattern);
    ret_easy = cre2_easy_match(pattern, pattern_len, text, text_len, match, nmatch);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)ret_set_add + (long)ret_easy
               + (long)(match[0].length) + (long)(match[1].length) + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}