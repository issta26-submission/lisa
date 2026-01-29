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
//<ID> 190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "User42 logged in";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t matches[4];
    const char * found_name = NULL;
    int found_index = 0;
    int nmatch = 3;
    int match_res = 0;
    bool iter_ret = false;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    matches[0].data = NULL;
    matches[0].length = 0;
    matches[1].data = NULL;
    matches[1].length = 0;
    matches[2].data = NULL;
    matches[2].length = 0;
    re = cre2_new(pattern, pattern_len, NULL);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Core operations
    match_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nmatch);
    iter_ret = cre2_named_groups_iter_next(iter, &found_name, &found_index);

    // step 4: Inspect and propagate results (no branching)
    matches[0].length = matches[0].length + ((match_res + (int)iter_ret + found_index) & 7);
    matches[1].length = matches[1].length + ((found_index ^ match_res) & 3);
    found_index = found_index + ((int)matches[0].length & 0xF);
    (void)found_name;
    (void)pattern_len;
    (void)input_text_len;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // step 6: API sequence test completed successfully
    return 66;
}