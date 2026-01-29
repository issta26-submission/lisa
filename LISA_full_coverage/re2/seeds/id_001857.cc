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
//<ID> 1857
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
    const char source_text[] = "xx foobar yy";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_buf[128];
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, source_text, (size_t)source_text_len);

    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;

    cre2_string_t match_arr[4];
    cre2_string_t partial_arr[4];
    memset(match_arr, 0, sizeof(match_arr));
    memset(partial_arr, 0, sizeof(partial_arr));

    cre2_string_t text_input;
    text_input.data = text_buf;
    text_input.length = source_text_len;

    int match_res = 0;
    int partial_res = 0;
    int perl_flag = 0;
    int one_line_flag = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Configure
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations
    match_arr[0].data = text_buf;
    match_arr[0].length = 0;
    match_arr[1].data = text_buf;
    match_arr[1].length = 0;
    match_arr[2].data = text_buf;
    match_arr[2].length = 0;
    partial_arr[0].data = text_buf;
    partial_arr[0].length = 0;
    partial_arr[1].data = text_buf;
    partial_arr[1].length = 0;
    partial_arr[2].data = text_buf;
    partial_arr[2].length = 0;
    match_res = cre2_match(re, text_buf, source_text_len, 0, source_text_len, CRE2_UNANCHORED, match_arr, 3);
    partial_res = cre2_partial_match(pattern, &text_input, partial_arr, 3);

    // step 5: Validate
    perl_flag = cre2_opt_perl_classes(opt);
    one_line_flag = cre2_opt_one_line(opt);
    checksum = (long)pattern_len + (long)source_text_len + (long)match_res + (long)partial_res + (long)perl_flag + (long)one_line_flag + (long)(re != NULL) + (long)(set != NULL) + (long)match_arr[0].length + (long)partial_arr[0].length;
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}