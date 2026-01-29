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
//<ID> 1136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]+)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foo123bar suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_string_t input;
    struct cre2_string_t matches[3];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int ret_partial = 0;
    int ret_consume = 0;
    const char * compiled_pat = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Compile / inspect pattern
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pat = cre2_pattern(re);

    // step 4: Prepare input and match buffers
    input.data = text_buf;
    input.length = text_len;
    memset(matches, 0, sizeof(matches));

    // step 5: Core operations (call required APIs)
    ret_partial = cre2_partial_match(pattern, &input, matches, nmatch);
    ret_consume = cre2_consume(pattern, &input, matches, nmatch);

    // step 6: Validate / Cleanup
    computed_check = (long)ret_partial + (long)ret_consume + (long)matches[0].length + (long)pattern_len + (long)input.length;
    (void)computed_check;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}