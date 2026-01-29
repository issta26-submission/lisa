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
//<ID> 193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?<g1>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "prefix foobar suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const int nmatch = 3;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t matches[4];
    const char * ng_name = NULL;
    int ng_index = -1;
    int easy_res = 0;
    int propagate = 0;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    matches[0].data = input_text;
    matches[0].length = 0;
    matches[1].data = NULL;
    matches[1].length = 0;
    matches[2].data = NULL;
    matches[2].length = 0;

    // step 3: Core operations (compile and easy match)
    re = cre2_new(pattern, pattern_len, (const cre2_options_t *)NULL);
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nmatch);

    // step 4: Named groups inspection (single advancement)
    iter = cre2_named_groups_iter_new(re);
    (void)cre2_named_groups_iter_next(iter, &ng_name, &ng_index);
    propagate = (easy_res + ng_index) & 0xF;

    // step 5: Validate / propagate results (use values meaningfully; no branching)
    matches[0].length = matches[0].length + ((easy_res + propagate) & 7);
    matches[1].length = matches[1].length + ((ng_index + propagate) & 3);
    matches[2].length = matches[2].length + ((easy_res ^ ng_index) & 1);
    (void)pattern_len;
    (void)input_text_len;
    (void)ng_name;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}