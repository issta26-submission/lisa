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
//<ID> 328
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char sample_text[] = "hello_world";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    cre2_options_t * opt = NULL;
    int perl_flag = 1;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    char output[8];
    int len0 = 0;
    int len1 = 0;

    // step 2: Setup
    memset(output, 0, sizeof(output));
    memset(ranges, 0, sizeof(ranges));
    strings[0].data = sample_text;
    strings[0].length = 5; /* "hello" */
    strings[1].data = sample_text + 6;
    strings[1].length = 5; /* "world" */

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_perl_classes(opt, perl_flag);

    // step 4: Operate
    cre2_strings_to_ranges(sample_text, ranges, strings, 2);

    // step 5: Validate
    len0 = (int)(ranges[0].past - ranges[0].start);
    len1 = (int)(ranges[1].past - ranges[1].start);
    output[0] = (char)(len0 & 0xFF);
    output[1] = (char)(len1 & 0xFF);
    output[2] = (char)(perl_flag & 0xFF);

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}