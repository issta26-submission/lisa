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
//<ID> 755
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foobar tail";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * pat_from_re = NULL;
    cre2_string_t input;
    cre2_string_t groups[3];
    int rc_consume = 0;
    int patlen_from_api = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pat_from_re = cre2_pattern(re);
    input.data = text_buf;
    input.length = text_len;
    groups[0].data = NULL; groups[0].length = 0;
    groups[1].data = NULL; groups[1].length = 0;
    groups[2].data = NULL; groups[2].length = 0;
    rc_consume = cre2_consume_re(re, &input, groups, 3);

    // step 4: Validate
    patlen_from_api = (int)strlen(pat_from_re);
    computed_check = rc_consume + input.length + groups[0].length + groups[1].length + patlen_from_api;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}