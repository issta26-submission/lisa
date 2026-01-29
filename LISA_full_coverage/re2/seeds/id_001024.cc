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
//<ID> 1024
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
    char text_buf[] = "foo123bar foo456baz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match_buf0[64];
    char match_buf1[64];
    char match_buf2[64];
    cre2_options_t * opt = NULL;
    cre2_string_t text;
    cre2_string_t text_consume;
    cre2_string_t matches[3];
    int partial_res = 0;
    int consume_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations - prepare input and match buffers, perform partial match
    memset(match_buf0, 0, sizeof(match_buf0));
    memset(match_buf1, 0, sizeof(match_buf1));
    memset(match_buf2, 0, sizeof(match_buf2));
    text.data = text_buf;
    text.length = text_len;
    matches[0].data = match_buf0;
    matches[0].length = 0;
    matches[1].data = match_buf1;
    matches[1].length = 0;
    matches[2].data = match_buf2;
    matches[2].length = 0;
    partial_res = cre2_partial_match(pattern, &text, matches, 3);

    // step 4: Additional operations - attempt to consume from the same text
    text_consume = text;
    consume_res = cre2_consume(pattern, &text_consume, matches, 3);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)partial_res + (long)consume_res + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length + (long)text.length + (long)pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}