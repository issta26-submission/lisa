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
//<ID> 1131
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]{2})bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo12bar and more";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * compiled_pattern = NULL;
    cre2_string_t text;
    cre2_string_t matches[3];
    int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int partial_ret = 0;
    int consume_ret = 0;
    long computed_check = 0;

    // step 2: Setup options
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Compile regexp and obtain compiled pattern pointer
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pattern = cre2_pattern(re);

    // step 4: Prepare input and match structures
    text.data = text_buf;
    text.length = text_len;
    memset(matches, 0, sizeof(matches));

    // step 5: Operate: partial match then consume using the pattern(s)
    partial_ret = cre2_partial_match(compiled_pattern, &text, matches, nmatch);
    consume_ret = cre2_consume(pattern, &text, matches, nmatch);

    // step 6: Validate and Cleanup
    computed_check = (long)partial_ret + (long)consume_ret + (long)matches[0].length + (long)pattern_len + (long)text.length;
    (void)computed_check;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}