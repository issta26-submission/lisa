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
//<ID> 2113
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]*)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    struct cre2_set *set = NULL;
    char errbuf[128];
    int add_rc = 0;
    int ncap = 0;
    int errcode = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Configure (compile regexp and create a set)
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 4: Operate (prepare buffers and add pattern to set)
    memset(errbuf, 0, sizeof(errbuf));
    add_rc = cre2_set_add(set, pattern, (size_t)pattern_len, errbuf, sizeof(errbuf));

    // step 5: Validate (query regexp and combine values so results flow)
    ncap = cre2_num_capturing_groups(re);
    errcode = cre2_error_code(re);
    checksum = (long)pattern_len + (long)add_rc + (long)ncap + (long)errcode + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL) + (long)errbuf[0];
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}