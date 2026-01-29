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
//<ID> 1107
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo123 bar foo456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_obj;
    cre2_string_t matches[4];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int wb_flag = 0;
    int pm_ret = 0;
    int fac_ret = 0;
    const char * re_pat = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    wb_flag = cre2_opt_word_boundary(opt);
    re = cre2_new(pattern, pattern_len, opt);
    memset(matches, 0, sizeof(matches));
    text_obj.data = text_buf;
    text_obj.length = text_len;

    // step 3: Core operations - perform a partial match using the pattern string API
    pm_ret = cre2_partial_match(pattern, &text_obj, matches, nmatch);

    // step 4: Core operations - find and consume a match from the same text object
    fac_ret = cre2_find_and_consume(pattern, &text_obj, matches, nmatch);

    // step 5: Validate / compute lightweight checksum and inspect compiled regexp pattern
    re_pat = cre2_pattern(re);
    computed_check = (long)wb_flag + (long)pm_ret + (long)fac_ret + (long)matches[0].length + (long)text_obj.length + (long)pattern_len;
    (void)computed_check;
    (void)re_pat;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}