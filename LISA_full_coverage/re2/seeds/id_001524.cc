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
//<ID> 1524
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<g>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t input;
    cre2_string_t match;
    char inputbuf[] = "prefix foobar suffix";
    char matchbuf[64];
    int partial_ret = 0;
    int named_index = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&input, 0, sizeof(input));
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_log_errors(opt, 1);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    input.data = inputbuf;
    input.length = (int)(sizeof(inputbuf) - 1);
    match.data = matchbuf;
    match.length = 0;
    partial_ret = cre2_partial_match(pattern, &input, &match, 2);
    named_index = cre2_find_named_capturing_groups(re, "g");

    // step 5: Validate
    checksum = (long)pattern_len + (long)partial_ret + (long)named_index + (long)(re != NULL) + (long)(iter != NULL) + (long)input.length + (long)match.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}