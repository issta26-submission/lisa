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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char * pattern = "(ab)(cd)?";
    int pattern_len = (int) (sizeof("(ab)(cd)?") - 1);
    const char * input_text = "abcd ab abcd";
    int input_len = (int) (sizeof("abcd ab abcd") - 1);
    cre2_regexp_t * rex = cre2_new(pattern, pattern_len, NULL);
    cre2_string_t input_str;
    input_str.data = input_text;
    input_str.length = input_len;
    cre2_string_t full_matches[3];
    cre2_range_t ranges[3];
    memset(full_matches, 0, sizeof(full_matches));
    memset(ranges, 0, sizeof(ranges));
    // step 2: Setup (Configure)
    cre2_string_t easy_matches[2];
    memset(easy_matches, 0, sizeof(easy_matches));
    // step 3: Operate (use full match with compiled regex and easy match)
    cre2_full_match_re(rex, &input_str, full_matches, 3);
    cre2_easy_match("ab", 2, input_text, input_len, easy_matches, 2);
    // step 4: Transform (strings to ranges) and Replace (operate on text buffer)
    cre2_strings_to_ranges(input_text, ranges, full_matches, 3);
    int buf_len = input_len + 128;
    char * buf = new char[buf_len];
    memset(buf, 0, buf_len);
    memcpy(buf, input_text, input_len);
    cre2_string_t text_and_target;
    text_and_target.data = buf;
    text_and_target.length = input_len;
    cre2_string_t rewrite;
    rewrite.data = "X";
    rewrite.length = 1;
    cre2_replace_re(rex, &text_and_target, &rewrite);
    // step 5: Cleanup (Destroy resources)
    cre2_delete(rex);
    delete [] buf;
    // API sequence test completed successfully
    return 66;
}