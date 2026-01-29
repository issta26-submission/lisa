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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int prog_size = 0;
    int initial_one_line = 1;
    int initial_case = 1;

    // step 2: Setup - create options
    opt = cre2_opt_new();

    // step 3: Configure options
    cre2_opt_set_one_line(opt, initial_one_line);
    cre2_opt_set_case_sensitive(opt, initial_case);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate - compile regex and query program size
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);

    // step 5: Validate / further configure using obtained values (data flows)
    cre2_opt_set_one_line(opt, (prog_size > 0));
    cre2_opt_set_case_sensitive(opt, (enc == CRE2_UTF8));

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}