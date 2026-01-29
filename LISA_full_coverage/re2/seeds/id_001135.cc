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
//<ID> 1135
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]*)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Token123 Extra";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * compiled_pat = NULL;
    cre2_string_t subject;
    cre2_string_t matches[3];
    int partial_ret = 0;
    int consume_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations - compile pattern and obtain compiled pattern string
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pat = cre2_pattern(re);

    // step 4: Prepare subject and match storage, then perform partial match
    subject.data = text_buf;
    subject.length = text_len;
    memset(matches, 0, sizeof(matches));
    partial_ret = cre2_partial_match(pattern, &subject, matches, (int)(sizeof(matches) / sizeof(matches[0])));

    // step 5: Perform consume using the same pattern and subject to advance/consume and capture groups
    consume_ret = cre2_consume(pattern, &subject, matches, (int)(sizeof(matches) / sizeof(matches[0])));

    // step 6: Validate / Cleanup
    computed_check = (long)partial_ret + (long)consume_ret + (long)matches[0].length + (long)pattern_len + (long)text_len + (long)(compiled_pat ? 1 : 0);
    (void)computed_check;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}