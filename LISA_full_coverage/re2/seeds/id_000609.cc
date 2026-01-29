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
//<ID> 609
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(alpha)(beta)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int interface_ver = 0;
    int perl_classes_flag = 0;
    int literal_flag = 1;
    int n_capturing = 0;
    int errcode = 0;
    char scratch[32];
    int computed_check = 0;

    // step 2: Setup
    memset(scratch, 0, sizeof(scratch));
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, literal_flag);

    // step 3: Configure
    perl_classes_flag = cre2_opt_perl_classes(opt);
    interface_ver = cre2_version_interface_current();

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Validate
    n_capturing = cre2_num_capturing_groups(re);
    errcode = cre2_error_code(re);
    computed_check = interface_ver + perl_classes_flag + literal_flag + n_capturing + errcode + pattern_len + (int)scratch[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}