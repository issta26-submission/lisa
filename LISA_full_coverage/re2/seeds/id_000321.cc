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
//<ID> 321
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char text[] = "foobarbaz";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    char result_buf[16];
    int nmatch = 2;
    int perl_flag = 1;

    // step 2: Setup
    memset(result_buf, 0, sizeof(result_buf));
    memset(ranges, 0, sizeof(ranges));
    strings[0].data = text;
    strings[0].length = 3; /* "foo" */
    strings[1].data = text + 3;
    strings[1].length = 3; /* "bar" */

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, perl_flag);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Operate / Validate
    cre2_strings_to_ranges(text, ranges, strings, nmatch);
    /* Use the computed ranges to influence the result buffer */
    result_buf[0] = (char)((int)ranges[0].start + (int)ranges[0].past + (int)ranges[1].start + (int)ranges[1].past + nmatch + perl_flag);
    result_buf[1] = (char)((ranges[0].past - ranges[0].start) + (ranges[1].past - ranges[1].start));

    // step 5: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}