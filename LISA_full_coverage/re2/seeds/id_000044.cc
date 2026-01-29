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
//<ID> 44
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)( world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "hello world";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_regexp_t * re = NULL;
    cre2_string_t easy_match;
    cre2_string_t full_matches[3];
    cre2_string_t text_str;
    int easy_res = 0;
    int full_res = 0;
    int prog_size = 0;

    // step 2: Setup
    memset(&easy_match, 0, sizeof(easy_match));
    memset(full_matches, 0, sizeof(full_matches));
    memset(&text_str, 0, sizeof(text_str));
    text_str.data = text;
    text_str.length = text_len;

    // step 3: Configure (compile the pattern)
    re = cre2_new(pattern, pattern_len, NULL);

    // step 4: Operate (easy match, full match, query program size)
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &easy_match, 2);
    full_res = cre2_full_match(pattern, &text_str, full_matches, 3);
    prog_size = cre2_program_size(re);

    // step 5: Validate (use results to avoid unused warnings)
    (void)easy_res;
    (void)full_res;
    (void)prog_size;
    (void)easy_match;
    (void)full_matches;
    (void)text_str;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}