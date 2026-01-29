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
//<ID> 1027
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern_consume[] = "([A-Za-z]+)([0-9]+):";
    const char pattern_partial[] = "\\w+";
    const int pattern_consume_len = (int)(sizeof(pattern_consume) - 1);
    const int pattern_partial_len = (int)(sizeof(pattern_partial) - 1);
    char text_buf[] = "alice123:remaining_data";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char consume_match_buf[128];
    char partial_match_buf[128];
    cre2_options_t * opt = NULL;
    cre2_string_t input;
    cre2_string_t consume_match;
    cre2_string_t partial_match;
    int consume_res = 0;
    int partial_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations
    memset(consume_match_buf, 0, sizeof(consume_match_buf));
    memset(partial_match_buf, 0, sizeof(partial_match_buf));
    input.data = text_buf;
    input.length = text_len;
    consume_match.data = consume_match_buf;
    consume_match.length = 0;
    partial_match.data = partial_match_buf;
    partial_match.length = 0;
    consume_res = cre2_consume(pattern_consume, &input, &consume_match, 2);
    partial_res = cre2_partial_match(pattern_partial, &input, &partial_match, 1);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)consume_res + (long)partial_res + (long)consume_match.length + (long)partial_match.length + (long)input.length + (long)pattern_consume_len + (long)pattern_partial_len;
    (void)computed_check;

    // step 5: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}