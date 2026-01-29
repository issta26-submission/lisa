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
//<ID> 1025
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "alice123 bob456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char text_copy_buf[] = "alice123 bob456";
    const int text_copy_len = (int)(sizeof(text_copy_buf) - 1);
    char match_buf1[64];
    char match_buf2[64];
    char target_buf1[64];
    char target_buf2[64];
    cre2_options_t * opt = NULL;
    cre2_string_t text;
    cre2_string_t text_copy;
    cre2_string_t matches[2];
    cre2_string_t targets[2];
    int partial_res = 0;
    int consume_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    memset(match_buf1, 0, sizeof(match_buf1));
    memset(match_buf2, 0, sizeof(match_buf2));
    memset(target_buf1, 0, sizeof(target_buf1));
    memset(target_buf2, 0, sizeof(target_buf2));

    // step 3: Core operations (prepare cre2_string_t structures and invoke APIs)
    text.data = text_buf;
    text.length = text_len;
    matches[0].data = match_buf1;
    matches[0].length = 0;
    matches[1].data = match_buf2;
    matches[1].length = 0;
    partial_res = cre2_partial_match(pattern, &text, matches, 2);

    text_copy.data = text_copy_buf;
    text_copy.length = text_copy_len;
    targets[0].data = target_buf1;
    targets[0].length = 0;
    targets[1].data = target_buf2;
    targets[1].length = 0;
    consume_res = cre2_consume(pattern, &text_copy, targets, 2);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)partial_res + (long)consume_res + (long)matches[0].length + (long)matches[1].length + (long)targets[0].length + (long)targets[1].length + (long)text.length + (long)text_copy.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}