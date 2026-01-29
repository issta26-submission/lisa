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
//<ID> 1102
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
    char text_buf[] = "foo123bar";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_obj;
    cre2_string_t matches[4];
    cre2_string_t consumed;
    const char * re_pattern_from_re = NULL;
    int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int word_boundary_flag = 0;
    int ret_partial = 0;
    int ret_find_and_consume = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    word_boundary_flag = cre2_opt_word_boundary(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    memset(matches, 0, sizeof(matches));
    memset(&consumed, 0, sizeof(consumed));
    text_obj.data = text_buf;
    text_obj.length = text_len;
    ret_partial = cre2_partial_match(pattern, &text_obj, matches, nmatch);
    ret_find_and_consume = cre2_find_and_consume(pattern, &text_obj, &consumed, 0);

    // step 4: Validate / compute lightweight checksum
    re_pattern_from_re = cre2_pattern(re);
    computed_check = (long)word_boundary_flag + (long)ret_partial + (long)ret_find_and_consume + (long)(matches[0].length) + (long)(consumed.length) + (long)pattern_len + (long)text_len + (long)(re_pattern_from_re ? 0 : 0);
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}