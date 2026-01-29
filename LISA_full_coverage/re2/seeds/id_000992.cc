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
//<ID> 992
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int add_res = 0;
    long computed_check = 0;

    // step 2: Setup - create and configure options, create set and compile regexp
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations - add pattern to set and create named-groups iterator
    add_res = cre2_set_add_simple(set, pattern);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)add_res + (long)enc + (long)pattern_len + (re != NULL ? 1L : 0L) + (set != NULL ? 2L : 0L) + (iter != NULL ? 3L : 0L);
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}