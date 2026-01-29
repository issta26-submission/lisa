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
//<ID> 1144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^([A-Za-z_]+)-(\\d+)$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int program_sz = 0;
    int log_errors_flag = 0;
    int perl_classes_flag = 1;
    char scratch[32];
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_perl_classes(opt, perl_classes_flag);
    memset(scratch, 0, sizeof(scratch));

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    program_sz = cre2_program_size(re);
    log_errors_flag = cre2_opt_log_errors(opt);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)program_sz + (long)log_errors_flag + (long)pattern_len + (long)perl_classes_flag + (long)(unsigned char)scratch[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}