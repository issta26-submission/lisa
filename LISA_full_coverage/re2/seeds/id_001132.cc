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
//<ID> 1132
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "User-42 and more text";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * compiled_pattern = NULL;
    cre2_string_t subject;
    cre2_string_t subject2;
    cre2_string_t matches[3];
    cre2_string_t matches2[3];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int ret_partial = 0;
    int ret_consume = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Compile / prepare pattern
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pattern = cre2_pattern(re);

    // step 4: Operate - prepare subjects and perform partial match and consume
    subject.data = text_buf;
    subject.length = text_len;
    subject2.data = text_buf;
    subject2.length = text_len;
    memset(matches, 0, sizeof(matches));
    memset(matches2, 0, sizeof(matches2));
    ret_partial = cre2_partial_match(compiled_pattern, &subject, matches, nmatch);
    ret_consume = cre2_consume(compiled_pattern, &subject2, matches2, nmatch);

    // step 5: Validate / lightweight checksum
    computed_check = (long)ret_partial + (long)ret_consume + (long)matches[0].length + (long)matches2[0].length + (long)pattern_len + (long)text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}