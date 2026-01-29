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
//<ID> 1020
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "ABC123def456GHI789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match_buf[64];
    char consumed_buf[64];
    cre2_options_t * opt = NULL;
    cre2_string_t input;
    cre2_string_t match;
    cre2_string_t consumed;
    int partial_res = 0;
    int consume_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Prepare cre2_string_t structures
    memset(match_buf, 0, sizeof(match_buf));
    memset(consumed_buf, 0, sizeof(consumed_buf));
    input.data = text_buf;
    input.length = text_len;
    match.data = match_buf;
    match.length = 0;
    consumed.data = consumed_buf;
    consumed.length = 0;

    // step 4: Core operations - partial match (non-consuming) then consume (destructive)
    partial_res = cre2_partial_match(pattern, &input, &match, 2);
    consume_res = cre2_consume(pattern, &input, &consumed, 2);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)partial_res + (long)consume_res + (long)match.length + (long)consumed.length + (long)input.length + (long)pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}