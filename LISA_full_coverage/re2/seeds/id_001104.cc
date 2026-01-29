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
//<ID> 1104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)\\b([0-9]*)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "token123 other token456 end";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_obj;
    cre2_string_t matches[4];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int wb_flag = 0;
    int partial_ret = 0;
    int find_consume_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    wb_flag = cre2_opt_word_boundary(opt);
    memset(matches, 0, sizeof(matches));
    text_obj.data = text_buf;
    text_obj.length = text_len;

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    const char * pat_from_re = cre2_pattern(re);

    // step 4: Operate
    partial_ret = cre2_partial_match(pat_from_re, &text_obj, matches, nmatch);
    find_consume_ret = cre2_find_and_consume(pat_from_re, &text_obj, matches, nmatch);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)wb_flag + (long)partial_ret + (long)find_consume_ret + (long)pattern_len + (long)text_len + (long)matches[0].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}