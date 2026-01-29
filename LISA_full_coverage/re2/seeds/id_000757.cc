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
//<ID> 757
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char pattern[] = "(foo)([0-9]+)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char text_buf[] = "prefix foo123bar suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t input;
    cre2_string_t matches[4];
    int rc_consume = 0;
    const char * recovered_pattern = NULL;
    int computed_check = 0;

    // step 2: Configure
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Operate (create regexp and perform consume)
    re = cre2_new(pattern, pattern_len, opt);
    recovered_pattern = cre2_pattern(re);

    input.data = text_buf;
    input.length = text_len;

    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;
    matches[2].data = NULL; matches[2].length = 0;
    matches[3].data = NULL; matches[3].length = 0;

    rc_consume = cre2_consume_re(re, &input, matches, 4);

    // step 4: Validate (derive a simple checksum to ensure values flowed)
    computed_check = rc_consume + (int)input.length + matches[0].length + matches[1].length + matches[2].length + (int)recovered_pattern[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}