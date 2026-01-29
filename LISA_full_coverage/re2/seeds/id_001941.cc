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
//<ID> 1941
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "Hello (world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "Hello world";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[2];
    int match_ret = 0;
    int one_line_flag = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    one_line_flag = cre2_opt_one_line(opt);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (prepare match slots and perform match)
    matches[0].data = NULL;
    matches[0].length = 0;
    matches[1].data = NULL;
    matches[1].length = 0;
    match_ret = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 2);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)match_ret + (long)matches[0].length + (long)matches[1].length + (long)one_line_flag + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}