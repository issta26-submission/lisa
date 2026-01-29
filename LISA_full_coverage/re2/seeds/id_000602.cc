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
//<ID> 602
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)(c)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    char scratch[16];
    int literal_set = 1;
    int perl_flag = 0;
    int num_groups = 0;
    int version = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(scratch, 0, sizeof(scratch));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, literal_set);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Validate
    perl_flag = cre2_opt_perl_classes(opt);
    num_groups = cre2_num_capturing_groups(re);
    version = cre2_version_interface_current();
    computed_check = version + num_groups + perl_flag + literal_set + pattern_len + (int)scratch[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    // API sequence test completed successfully
    return 66;
}