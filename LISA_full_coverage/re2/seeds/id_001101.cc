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
//<ID> 1101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "\\s*(\\w+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "abc-123 def-456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_obj;
    cre2_string_t matches[3];
    cre2_string_t match_single;
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int initial_word_boundary = 0;
    int consume_ret = 0;
    int partial_ret = 0;
    long computed_check = 0;

    // step 2: Setup / Configure
    opt = cre2_opt_new();
    initial_word_boundary = cre2_opt_word_boundary(opt);
    re = cre2_new(pattern, pattern_len, opt);
    memset(matches, 0, sizeof(matches));
    match_single.data = NULL;
    match_single.length = 0;

    // step 3: Core operations
    text_obj.data = text_buf;
    text_obj.length = text_len;
    {
        const char * pat_from_re = cre2_pattern(re);
        consume_ret = cre2_find_and_consume(pat_from_re, &text_obj, &match_single, 1);
        partial_ret = cre2_partial_match(pat_from_re, &text_obj, matches, nmatch);
    }

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)initial_word_boundary + (long)consume_ret + (long)partial_ret + (long)pattern_len + (long)text_len + (long)matches[0].length + (long)match_single.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}