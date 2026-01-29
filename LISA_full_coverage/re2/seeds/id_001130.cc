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
//<ID> 1130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Token-123 extra";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * compiled_pattern = NULL;
    cre2_string_t input;
    cre2_string_t consume_text;
    cre2_string_t input_array[1];
    cre2_string_t matches[3];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int partial_ret = 0;
    int consume_ret = 0;
    long computed_check = 0;

    // step 2: Setup (initialize options and configure)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Operate (compile pattern and prepare string structures)
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pattern = cre2_pattern(re);
    input.data = text_buf;
    input.length = text_len;
    consume_text = input;
    input_array[0] = input;
    memset(matches, 0, sizeof(matches));
    memset(input_array + 1, 0, 0); // harmless no-op to avoid unused warnings

    // step 4: Core API calls (partial match using compiled pattern string, then consume)
    partial_ret = cre2_partial_match(compiled_pattern, input_array, matches, nmatch);
    consume_ret = cre2_consume(compiled_pattern, &consume_text, matches, nmatch);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)partial_ret + (long)consume_ret + (long)matches[0].length + (long)input.length + (long)pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}