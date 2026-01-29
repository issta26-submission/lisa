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
//<ID> 1991
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>abc)def";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * returned_pattern = NULL;
    int perl_classes_flag = 0;
    int first_char_val = 0;
    long checksum = 0;
    char scratch[32];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup - create and configure options
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)(1 << 20));
    perl_classes_flag = cre2_opt_perl_classes(opt);

    // step 3: Operate - compile pattern with options
    re = cre2_new(pattern, pattern_len, opt);
    returned_pattern = cre2_pattern(re);

    // step 4: Validate - compute a checksum using results of API calls
    first_char_val = returned_pattern ? (int)returned_pattern[0] : 0;
    checksum = (long)pattern_len + (long)perl_classes_flag + (long)first_char_val + (long)(opt != NULL) + (long)(re != NULL) + (long)((1 << 20) & 0xFFFF);
    (void)checksum;
    
    // step 5: Cleanup - free resources
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}