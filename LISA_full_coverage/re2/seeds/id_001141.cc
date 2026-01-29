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
//<ID> 1141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?:test)([0-9]{1,4})";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int log_errors_flag = 0;
    int perl_classes_flag = 1;
    int prog_size = 0;
    char scratch[16];
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_perl_classes(opt, perl_classes_flag);
    cre2_opt_set_log_errors(opt, 1);
    log_errors_flag = cre2_opt_log_errors(opt);

    // step 3: Core operations
    memset(scratch, 0, sizeof(scratch));
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)prog_size + (long)log_errors_flag + (long)perl_classes_flag + (long)(unsigned char)scratch[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}