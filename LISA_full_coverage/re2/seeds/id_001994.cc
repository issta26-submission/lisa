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
//<ID> 1994
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo[0-9]+";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int perl_classes_flag = 0;
    int64_t max_memory_bytes = (int64_t) (16 * 1024 * 1024); // 16MB
    long checksum = 0;
    char workspace[64];
    memset(workspace, 0, sizeof(workspace));

    // step 2: Setup - create and configure options
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_max_mem(opt, max_memory_bytes);
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Inspect configuration
    perl_classes_flag = cre2_opt_perl_classes(opt);

    // step 4: Operate - compile the pattern with options
    re = cre2_new(pattern, pattern_len, opt);
    const char * compiled_pattern = cre2_pattern(re);

    // step 5: Validate computed state (combine meaningful values)
    checksum = (long)pattern_len + (long)perl_classes_flag + (long)(compiled_pattern != NULL) + (long)(opt != NULL) + (long)(re != NULL) + (long)(max_memory_bytes & 0xFFFF);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}