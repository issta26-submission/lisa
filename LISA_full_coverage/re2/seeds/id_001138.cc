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
//<ID> 1138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "abc([0-9]+)xyz";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix abc123xyz suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * compiled_pattern = NULL;
    cre2_string_t text_str;
    cre2_string_t consume_text;
    cre2_string_t partial_matches[3];
    cre2_string_t consume_matches[3];
    const int nmatch = (int)(sizeof(partial_matches) / sizeof(partial_matches[0]));
    int partial_ret = 0;
    int consume_ret = 0;
    long computed_check = 0;

    // step 2: Setup options
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Compile pattern and prepare strings
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pattern = cre2_pattern(re);
    text_str.data = text_buf;
    text_str.length = text_len;
    consume_text = text_str;
    memset(partial_matches, 0, sizeof(partial_matches));
    memset(consume_matches, 0, sizeof(consume_matches));

    // step 4: Operate using partial match and consume APIs
    partial_ret = cre2_partial_match(compiled_pattern, &text_str, partial_matches, nmatch);
    consume_ret = cre2_consume(compiled_pattern, &consume_text, consume_matches, nmatch);

    // step 5: Validate and cleanup
    computed_check = (long)partial_ret + (long)consume_ret + (long)partial_matches[0].length + (long)consume_matches[0].length + (long)pattern_len + (long)text_len;
    (void)computed_check;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}