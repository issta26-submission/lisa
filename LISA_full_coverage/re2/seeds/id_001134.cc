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
//<ID> 1134
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Token-123 Extra";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input_partial;
    cre2_string_t input_consume;
    cre2_string_t out1;
    cre2_string_t out2;
    int nmatch = 2;
    int partial_ret = 0;
    int consume_ret = 0;
    const char * compiled_pattern = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Compile / inspect pattern
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pattern = cre2_pattern(re);

    // step 4: Prepare inputs and outputs
    input_partial.data = text_buf;
    input_partial.length = text_len;
    input_consume.data = text_buf;
    input_consume.length = text_len;
    memset(&out1, 0, sizeof(out1));
    memset(&out2, 0, sizeof(out2));

    // step 5: Core operations (partial match and consume)
    partial_ret = cre2_partial_match(pattern, &input_partial, &out1, nmatch);
    consume_ret = cre2_consume(pattern, &input_consume, &out2, nmatch);
    computed_check = (long)partial_ret + (long)consume_ret + (long)out1.length + (long)out2.length + (long)pattern_len + (long)text_len + (long)(compiled_pattern != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}