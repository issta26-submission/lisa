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
//<ID> 2093
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
    char text_buf[] = "Alice-12345";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t input;
    cre2_string_t matches[3];
    int partial_result = 0;
    int version_current = 0;
    int num_groups = 0;
    int64_t max_mem = 0;
    long checksum = 0;

    // step 2: Setup (initialize options and inspect limits)
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    max_mem = cre2_opt_max_mem(opt);

    // step 3: Configure (compile regexp with options)
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);

    // step 4: Operate (prepare input and attempt a partial match)
    input.data = text_buf;
    input.length = text_len;
    memset(matches, 0, sizeof(matches));
    partial_result = cre2_partial_match_re(re, &input, matches, 3);
    version_current = cre2_version_interface_current();

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)text_len + (long)partial_result + (long)version_current + (long)num_groups + (long)max_mem + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}