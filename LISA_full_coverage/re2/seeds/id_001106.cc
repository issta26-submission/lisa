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
//<ID> 1106
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "\\b([A-Za-z]+)\\b";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "hello world 123";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_obj;
    cre2_string_t matches[3];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int partial_ret = 0;
    int find_consume_ret = 0;
    const char * re_pat = NULL;
    int word_boundary_flag = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    word_boundary_flag = cre2_opt_word_boundary(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Prepare inputs
    text_obj.data = text_buf;
    text_obj.length = text_len;
    memset(matches, 0, sizeof(matches));

    // step 4: Core operations
    re_pat = cre2_pattern(re);
    partial_ret = cre2_partial_match(re_pat, &text_obj, matches, nmatch);
    find_consume_ret = cre2_find_and_consume(re_pat, &text_obj, matches, nmatch);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)partial_ret + (long)find_consume_ret + (long)word_boundary_flag + (long)matches[0].length + (long)text_obj.length + (long)pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}