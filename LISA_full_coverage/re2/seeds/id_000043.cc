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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(abc)(def)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "abcdef xyz abc";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t easy_match;
    cre2_string_t full_text;
    cre2_string_t groups[3];
    char buffer[64];
    int easy_res = 0;
    int full_res = 0;
    int prog_size = 0;

    // step 2: Setup
    memset(&easy_match, 0, sizeof(easy_match));
    memset(&full_text, 0, sizeof(full_text));
    memset(groups, 0, sizeof(groups));
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, text, (size_t)text_len);
    full_text.data = buffer;
    full_text.length = text_len;

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    easy_res = cre2_easy_match(pattern, pattern_len, buffer, text_len, &easy_match, 3);
    full_res = cre2_full_match(pattern, &full_text, groups, 3);
    prog_size = cre2_program_size(re);

    // step 5: Validate
    (void)easy_res;
    (void)full_res;
    (void)prog_size;
    (void)easy_match;
    (void)full_text;
    (void)groups;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}