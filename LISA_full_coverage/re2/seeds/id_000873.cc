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
//<ID> 873
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?s)(Foo)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * returned_pattern = NULL;
    int case_flag = 0;
    int dot_nl_flag = 0;
    long computed_check = 0;
    char scratch[16];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_dot_nl(opt, 1);
    cre2_opt_set_case_sensitive(opt, 0);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    returned_pattern = cre2_pattern(re);

    // step 4: Validate / compute lightweight checksum
    case_flag = cre2_opt_case_sensitive(opt);
    dot_nl_flag = cre2_opt_dot_nl(opt);
    computed_check = (long)pattern_len + (long)(returned_pattern[0]) + (long)case_flag + (long)dot_nl_flag;
    scratch[0] = (char)(computed_check & 0xFF);
    (void)scratch;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}