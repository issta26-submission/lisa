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
//<ID> 1133
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]+)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foo123bar suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_string_t text_str;
    struct cre2_string_t matches[3];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int partial_ret = 0;
    int consume_ret = 0;
    const char * compiled_pattern = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Operate - compile and prepare strings
    re = cre2_new(pattern, pattern_len, opt);
    text_str.data = text_buf;
    text_str.length = text_len;
    memset(matches, 0, sizeof(matches));
    partial_ret = cre2_partial_match(pattern, &text_str, matches, nmatch);

    // step 4: Validate / further operations
    compiled_pattern = cre2_pattern(re);
    consume_ret = cre2_consume(pattern, &text_str, matches, nmatch);
    computed_check = (long)partial_ret + (long)consume_ret + (long)matches[0].length + (long)text_str.length + (long)pattern_len;
    (void)compiled_pattern;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}