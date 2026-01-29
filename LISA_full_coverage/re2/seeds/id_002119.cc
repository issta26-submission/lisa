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
//<ID> 2119
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)-(\\d{2,4})";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    char set_errbuf[128];
    int add_result = 0;
    int num_caps = 0;
    int compile_err = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Configure (compile a regexp using the options)
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (inspect regexp and manipulate a set)
    num_caps = cre2_num_capturing_groups(re);
    compile_err = cre2_error_code(re);
    memset(set_errbuf, 0, sizeof(set_errbuf));
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_result = cre2_set_add(set, pattern, (size_t)pattern_len, set_errbuf, sizeof(set_errbuf));

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)num_caps + (long)compile_err + (long)add_result + (long)set_errbuf[0] + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}