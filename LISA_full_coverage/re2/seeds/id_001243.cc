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
//<ID> 1243
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo123 and bar45 end";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_regexp_t *re = NULL;
    const char *version = NULL;
    cre2_string_t input;
    cre2_string_t matches[4];
    int find_and_consume_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    version = cre2_version_string();
    memset(&input, 0, sizeof(input));
    memset(matches, 0, sizeof(matches));
    input.data = text_buf;
    input.length = text_len;

    // step 3: Create regex
    re = cre2_new(pattern, pattern_len, NULL);

    // step 4: Core operations
    find_and_consume_ret = cre2_find_and_consume(pattern, &input, matches, 4);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)find_and_consume_ret
                     + (long)input.length + (long)matches[0].length
                     + (long)(re != NULL) + (long)(version != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}