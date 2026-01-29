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
//<ID> 1245
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo123 and then foo456 later";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t match;
    const char * version = NULL;
    int find_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&input, 0, sizeof(input));
    memset(&match, 0, sizeof(match));
    input.data = text_buf;
    input.length = text_len;
    match.data = NULL;
    match.length = 0;
    version = cre2_version_string();
    re = cre2_new(pattern, pattern_len, (const cre2_options_t *)NULL);

    // step 3: Configure
    // (no explicit cre2_options_t used; pattern compiled above)

    // step 4: Core operations
    find_ret = cre2_find_and_consume(pattern, &input, &match, 2);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)find_ret + (long)match.length
                     + (long)(re != NULL) + (long)(version != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}