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
//<ID> 199
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "prefix foobar suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t matches[4];
    const char * ng_name = NULL;
    int ng_index = 0;
    int match_res = 0;
    const int nmatch = 4;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Core operations
    match_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nmatch);
    cre2_named_groups_iter_next(iter, &ng_name, &ng_index);

    // step 4: Inspect results
    matches[0].length = matches[0].length + ((match_res + ng_index) & 7);
    ng_index = ng_index + (matches[0].length & 3);
    (void)ng_name;
    (void)pattern_len;
    (void)input_text_len;

    // step 5: Validate / propagate results
    matches[1].length = matches[1].length + ((matches[0].length + match_res) & 3);
    matches[2].length = matches[2].length + ((ng_index ^ match_res) & 1);
    (void)matches;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}