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
//<ID> 1295
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "hello.*world";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int perl_classes_initial = 0;
    int perl_classes_after = 0;
    int64_t max_mem_before = 0;
    int64_t max_mem_after = 0;
    const char * re_pat = NULL;
    int prog_size = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    perl_classes_initial = cre2_opt_perl_classes(opt);
    max_mem_before = cre2_opt_max_mem(opt);

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)123456789);
    cre2_opt_set_perl_classes(opt, 1);
    perl_classes_after = cre2_opt_perl_classes(opt);
    max_mem_after = cre2_opt_max_mem(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    re_pat = cre2_pattern(re);
    prog_size = cre2_program_size(re);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)(re != NULL) + (long)prog_size
                     + (long)perl_classes_initial + (long)perl_classes_after
                     + (long)max_mem_before + (long)max_mem_after + (long)(re_pat != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}