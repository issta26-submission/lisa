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
//<ID> 2118
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
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    struct cre2_set *set = NULL;
    char errbuf[128];
    int add_result = 0;
    int compile_res = 0;
    int errcode = 0;
    int num_caps = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    memset(errbuf, 0, sizeof(errbuf));
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Configure (compile a regexp and create a set)
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 4: Operate (add pattern to set and compile the set)
    add_result = cre2_set_add(set, pattern, (size_t)pattern_len, errbuf, sizeof(errbuf));
    compile_res = cre2_set_compile(set);

    // step 5: Validate (inspect error code and number of capturing groups and combine values)
    errcode = cre2_error_code(re);
    num_caps = cre2_num_capturing_groups(re);
    checksum = (long)add_result + (long)compile_res + (long)errcode + (long)num_caps + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)errbuf[0];
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}