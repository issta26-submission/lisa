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
//<ID> 1384
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<g>foo)|bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * ng_name = NULL;
    int ng_index = -1;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int literal_flag = 0;
    bool next_ret = false;
    long computed_check = 0;

    // step 2: Setup
    memset(&computed_check, 0, sizeof(computed_check));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_literal(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    next_ret = cre2_named_groups_iter_next(iter, &ng_name, &ng_index);
    enc = cre2_opt_encoding(opt);
    literal_flag = cre2_opt_literal(opt);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)enc + (long)literal_flag
                     + (long)ng_index + (long)next_ret
                     + (long)(ng_name != NULL) + (long)(re != NULL)
                     + (long)(set != NULL) + (long)(opt != NULL);
    (void)computed_check;
    (void)ng_name;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}