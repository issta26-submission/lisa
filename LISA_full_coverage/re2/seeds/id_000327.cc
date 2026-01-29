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
//<ID> 327
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char sample_text[] = "hello world";
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    cre2_options_t * opt = NULL;
    cre2_encoding_t enc = CRE2_UTF8;
    int perl_flag = 1;
    char result_buf[4] = {0};
    int aggregated = 0;

    // step 2: Setup
    strings[0].data = sample_text;
    strings[0].length = 5;            // "hello"
    strings[1].data = sample_text + 6;
    strings[1].length = 5;            // "world"
    ranges[0].start = 0;
    ranges[0].past = 0;
    ranges[1].start = 0;
    ranges[1].past = 0;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_perl_classes(opt, perl_flag);
    cre2_opt_set_encoding(opt, enc);

    // step 4: Operate
    cre2_strings_to_ranges(sample_text, ranges, strings, 2);

    // step 5: Validate
    aggregated = (int)(ranges[0].start + ranges[0].past + ranges[1].start + ranges[1].past);
    result_buf[0] = (char)(aggregated & 0xFF);
    result_buf[1] = (char)(cre2_opt_perl_classes(opt) & 0xFF);

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}