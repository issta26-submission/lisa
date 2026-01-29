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
//<ID> 1026
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]+)";
    char text_buf[] = "alice123 bob456 carol789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match_buf1[64];
    char match_buf2[64];
    cre2_options_t * opt = NULL;
    cre2_string_t input;
    cre2_string_t match1;
    cre2_string_t match2;
    int partial_res = 0;
    int consume_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Prepare cre2_string_t structures for operations
    memset(match_buf1, 0, sizeof(match_buf1));
    memset(match_buf2, 0, sizeof(match_buf2));
    input.data = text_buf;
    input.length = text_len;
    match1.data = match_buf1;
    match1.length = 0;
    match2.data = match_buf2;
    match2.length = 0;

    // step 4: Operate - perform a partial match using the pattern and the input
    partial_res = cre2_partial_match(pattern, &input, &match1, 1);

    // step 5: Operate - consume from the same input (mutating semantics expected)
    consume_res = cre2_consume(pattern, &input, &match2, 1);

    // step 6: Validate / Cleanup
    computed_check = (long)partial_res + (long)consume_res + (long)input.length + (long)match1.length + (long)match2.length;
    (void)computed_check;
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}