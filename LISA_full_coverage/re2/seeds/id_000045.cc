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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "foobar";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t matches[3];
    int easy_res = 0;
    int full_res = 0;
    int prog_size = 0;

    // step 2: Setup
    memset(&input, 0, sizeof(input));
    memset(matches, 0, sizeof(matches));
    input.data = text;
    input.length = text_len;

    // step 3: Configure (compile the pattern)
    re = cre2_new(pattern, pattern_len, NULL);
    prog_size = cre2_program_size(re);

    // step 4: Operate (perform an easy match and a full match)
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &matches[0], 3);
    full_res = cre2_full_match(pattern, &input, matches, 3);

    // step 5: Validate (use results to ensure meaningful data flow)
    (void)easy_res;
    (void)full_res;
    (void)prog_size;
    (void)matches;
    (void)input;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}