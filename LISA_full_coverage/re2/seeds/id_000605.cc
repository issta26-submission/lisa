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
//<ID> 605
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(alpha)(beta)?(gamma)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int version_iface = 0;
    int literal_flag = 0;
    int perl_set_value = 1;
    int perl_flag_read = 0;
    int num_groups = 0;
    char scratch[32];
    int computed_check = 0;

    // step 2: Setup
    memset(scratch, 0, sizeof(scratch));
    version_iface = cre2_version_interface_current();
    literal_flag = version_iface & 1;
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, literal_flag);
    cre2_opt_set_perl_classes(opt, perl_set_value);

    // step 3: Configure
    perl_flag_read = cre2_opt_perl_classes(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);

    // step 5: Validate
    computed_check = version_iface + literal_flag + perl_set_value + perl_flag_read + num_groups + (int)scratch[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}