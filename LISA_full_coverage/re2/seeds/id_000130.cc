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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(abc|def)?g";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int case_flag = 0;
    int prog_size = 0;

    // step 2: Setup / Initialize
    opt = cre2_opt_new();
    enc = cre2_opt_encoding(opt);
    case_flag = (enc == CRE2_UTF8); // use encoding information to choose case sensitivity

    // step 3: Configure options
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_case_sensitive(opt, case_flag);

    // step 4: Operate - compile the regular expression
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Validate / Query - obtain program size from compiled regex
    prog_size = cre2_program_size(re);

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    (void)prog_size;
    (void)enc;
    (void)case_flag;
    (void)pattern_len;

    // API sequence test completed successfully
    return 66;
}