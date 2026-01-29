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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<g>hello)(world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "prefix helloworld suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const int nmatch = 4;
    cre2_string_t matches[4];
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * found_name = NULL;
    int found_index = -1;
    bool iter_next_res = false;
    int easy_res = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));

    // step 3: Core operations
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nmatch);
    re = cre2_new(pattern, pattern_len, (const cre2_options_t *)NULL);
    iter = cre2_named_groups_iter_new(re);
    iter_next_res = cre2_named_groups_iter_next(iter, &found_name, &found_index);

    // step 4: Inspect results / propagate state
    matches[0].length = matches[0].length + ((easy_res + found_index) & 7);
    matches[1].length = matches[1].length + (((int)iter_next_res) & 3);
    (void)found_name;
    (void)pattern_len;
    (void)input_text_len;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // step 6: API sequence test completed successfully
    return 66;
}