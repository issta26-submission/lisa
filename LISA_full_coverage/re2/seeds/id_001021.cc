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
//<ID> 1021
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([a-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "alice123bob456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match_buf[128];
    char match2_buf[128];
    cre2_options_t * opt = NULL;
    cre2_string_t input;
    cre2_string_t text_and_target;
    cre2_string_t match;
    cre2_string_t match2;
    int partial_res = 0;
    int consume_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Prepare buffers and string structs
    memset(match_buf, 0, sizeof(match_buf));
    memset(match2_buf, 0, sizeof(match2_buf));
    input.data = text_buf;
    input.length = text_len;
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    match.data = match_buf;
    match.length = 0;
    match2.data = match2_buf;
    match2.length = 0;

    // step 4: Core operations - partial match then consume using same pattern and text
    partial_res = cre2_partial_match(pattern, &input, &match, 2);
    consume_res = cre2_consume(pattern, &text_and_target, &match2, 2);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)partial_res + (long)consume_res + (long)match.length + (long)match2.length + (long)text_and_target.length + (long)pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}