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
//<ID> 1268
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]{2,4})";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foo123 suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text;
    cre2_string_t match;
    int set_add_ret = 0;
    int partial_ret = 0;
    const char * pat_from_re = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    memset(&text, 0, sizeof(text));
    memset(&match, 0, sizeof(match));
    text.data = text_buf;
    text.length = text_len;
    match.data = NULL;
    match.length = 0;

    // step 3: Configure
    cre2_opt_set_posix_syntax(opt, 1);
    set_add_ret = cre2_set_add_simple(set, pattern);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    pat_from_re = cre2_pattern(re);
    partial_ret = cre2_partial_match(pattern, &text, &match, 1);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)partial_ret
                     + (long)match.length + (long)set_add_ret + (long)(re != NULL)
                     + (long)(set != NULL) + (long)(opt != NULL) + (long)(pat_from_re != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}