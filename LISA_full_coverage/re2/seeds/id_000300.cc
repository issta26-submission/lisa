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
//<ID> 300
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    cre2_options_t * opt = NULL;
    int one_line_before = 0;
    int one_line_after = 0;
    int word_boundary_flag = 1;
    int perl_classes_flag = 1;
    char scratch[32];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup
    opt = cre2_opt_new();
    one_line_before = cre2_opt_one_line(opt);

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, word_boundary_flag);
    cre2_opt_set_perl_classes(opt, perl_classes_flag);

    // step 4: Operate / Validate
    one_line_after = cre2_opt_one_line(opt);
    scratch[0] = (char)((unsigned char)scratch[0] + (unsigned)((one_line_before & 0x3) + (one_line_after & 0x3) + (word_boundary_flag & 0x1) + (perl_classes_flag & 0x1)));

    // step 5: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}