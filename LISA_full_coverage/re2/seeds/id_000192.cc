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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>test)(ing)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "prefix testing suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t match;
    const char * named = NULL;
    int named_index = -1;
    int easy_res = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    match.data = NULL;
    match.length = 0;
    re = cre2_new(pattern, pattern_len, NULL);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Core operations
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &match, 3);
    cre2_named_groups_iter_next(iter, &named, &named_index);

    // step 4: Inspect / propagate results
    match.length = match.length + ((easy_res + (named_index & 0x7)) & 0x7);
    named_index = named_index + ((int)match.length & 3);
    match.data = named != NULL ? named : match.data;

    // step 5: Additional propagation (use values to influence cleanup behavior)
    (void)pattern_len;
    (void)input_text_len;
    (void)easy_res;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}