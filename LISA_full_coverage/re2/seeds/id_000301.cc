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
//<ID> 301
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char sample_text[] = "Sample CRE2 options usage";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char workspace[64];
    cre2_options_t * opt = NULL;
    int one_line_initial = 0;
    int computed_flag = 0;
    int one_line_after = 0;
    int accumulator = 0;

    // step 2: Setup
    memset(workspace, 0, sizeof(workspace));
    (void)sample_text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);

    // step 4: Operate
    one_line_initial = cre2_opt_one_line(opt);
    computed_flag = (one_line_initial & 1) ^ 1;
    cre2_opt_set_word_boundary(opt, computed_flag);
    cre2_opt_set_perl_classes(opt, computed_flag ^ 1);

    // step 5: Validate
    one_line_after = cre2_opt_one_line(opt);
    accumulator = (one_line_initial & 0x3) + (one_line_after & 0x3) + (computed_flag & 0x3);
    workspace[0] = (char)((unsigned char)workspace[0] + (unsigned)(accumulator & 0xFF));

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}