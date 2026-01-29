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
//<ID> 1375
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>hello)|world";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * pattern_from_re = NULL;
    int64_t max_mem_val = 0;
    int literal_flag = 0;
    int ver_iface = 0;
    long computed_check = 0;
    char scratch[16];
    
    // step 2: Setup
    memset(scratch, 0, sizeof(scratch));
    opt = cre2_opt_new();
    cre2_opt_set_max_mem(opt, (int64_t)1024 * 1024);
    cre2_opt_set_literal(opt, 1);
    
    // step 3: Core operations
    max_mem_val = cre2_opt_max_mem(opt);
    literal_flag = cre2_opt_literal(opt);
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);
    ver_iface = cre2_version_interface_current();
    
    // step 4: Validate
    computed_check = (long)pattern_len + (long)max_mem_val + (long)literal_flag
                     + (long)(re != NULL) + (long)(iter != NULL) + (long)(pattern_from_re != NULL)
                     + (long)ver_iface + (long)scratch[0];
    (void)computed_check;
    (void)pattern_from_re;
    
    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);
    
    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}