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
//<ID> 626
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char text[] = "prefix:hello:mid:world:suffix";
    const int nmatch = 2;
    cre2_options_t * opt = NULL;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int literal_flag = 1;
    int case_sensitive_flag = 0;
    cre2_encoding_t encoding = CRE2_UNKNOWN;
    int computed_check = 0;
    char scratch[8];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    /* prepare string views that point into `text` */
    strings[0].data = text + 7;  /* "hello" starts at index 7 */
    strings[0].length = 5;
    strings[1].data = text + 17; /* "world" starts at index 17 */
    strings[1].length = 5;

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, literal_flag);
    cre2_opt_set_case_sensitive(opt, 1);
    case_sensitive_flag = cre2_opt_case_sensitive(opt);
    encoding = cre2_opt_encoding(opt);

    // step 4: Operate
    cre2_strings_to_ranges(text, ranges, strings, nmatch);

    // step 5: Validate
    computed_check = (int)encoding + case_sensitive_flag + literal_flag
                     + (int)ranges[0].start + (int)ranges[0].past
                     + (int)ranges[1].start + (int)ranges[1].past
                     + strings[0].length + strings[1].length + (int)scratch[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}