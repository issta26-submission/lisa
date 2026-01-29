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
//<ID> 329
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char text[] = "hello world";
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    cre2_options_t * opt = NULL;
    int perl_flag = 1;
    cre2_encoding_t enc_set = CRE2_UTF8;
    int computed_sum = 0;

    // step 2: Setup
    memset(ranges, 0, sizeof(ranges));
    strings[0].data = text;
    strings[0].length = 5; /* "hello" */
    strings[1].data = text + 6;
    strings[1].length = 5; /* "world" */

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, perl_flag);
    cre2_opt_set_encoding(opt, enc_set);

    // step 4: Operate
    cre2_strings_to_ranges(text, ranges, strings, 2);

    // step 5: Validate
    computed_sum = (int)((long)strings[0].length + ranges[0].start + ranges[0].past + perl_flag + (int)enc_set);

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    (void)computed_sum;
    return 66;
}