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
//<ID> 1029
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
    char text_buf[] = "alice123 bob456 carol789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match0_buf[64];
    char match1_buf[64];
    cre2_options_t * opt = NULL;
    cre2_string_t subject;
    cre2_string_t matches[2];
    int consume_res = 0;
    int partial_res = 0;
    long computed_check = 0;

    // step 2: Setup (initialize options and buffers)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    memset(match0_buf, 0, sizeof(match0_buf));
    memset(match1_buf, 0, sizeof(match1_buf));

    // step 3: Prepare cre2_string_t structures
    subject.data = text_buf;
    subject.length = text_len;
    matches[0].data = match0_buf;
    matches[0].length = 0;
    matches[1].data = match1_buf;
    matches[1].length = 0;

    // step 4: Operate - consume from the start and attempt a partial match
    consume_res = cre2_consume(pattern, &subject, matches, 2);
    partial_res = cre2_partial_match(pattern, &subject, matches, 2);

    // step 5: Validate / compute lightweight checksum (use results and lengths meaningfully)
    computed_check = (long)consume_res + (long)partial_res + (long)matches[0].length + (long)matches[1].length + (long)pattern_len + (long)subject.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}