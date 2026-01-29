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
//<ID> 620
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char text[] = "Hello WORLD";
    const int text_len = (int)(sizeof(text) - 1);
    const int nmatch = 2;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    cre2_options_t * opt = NULL;
    int cs_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int literal_initial = 0;
    int literal_final = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    strings[0].data = text;
    strings[0].length = 5;         // "Hello"
    strings[1].data = text + 6;
    strings[1].length = 5;         // "WORLD"
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, literal_initial);

    // step 3: Configure
    cs_flag = cre2_opt_case_sensitive(opt);
    enc = cre2_opt_encoding(opt);
    literal_final = (cs_flag + (int)enc) & 1;
    cre2_opt_set_literal(opt, literal_final);

    // step 4: Operate
    cre2_strings_to_ranges(text, ranges, strings, nmatch);

    // step 5: Validate
    computed_check = (int)ranges[0].start + (int)ranges[0].past + (int)ranges[1].start + (int)ranges[1].past
                     + strings[0].length + strings[1].length + cs_flag + (int)enc + literal_final;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}