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
//<ID> 1868
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
    const char source_text[] = "xx foobar yy";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_buf[128];
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, source_text, (size_t)source_text_len);

    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;

    cre2_string_t matches[3];
    memset(matches, 0, sizeof(matches));
    matches[0].data = text_buf;
    matches[0].length = 0;
    matches[1].data = text_buf;
    matches[1].length = 0;
    matches[2].data = text_buf;
    matches[2].length = 0;

    cre2_string_t text_input;
    text_input.data = text_buf;
    text_input.length = source_text_len;

    char rewrite_buf[] = "$1";
    cre2_string_t rewrite_str;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = (int)(sizeof(rewrite_buf) - 1);

    char target_buf[128];
    memset(target_buf, 0, sizeof(target_buf));
    cre2_string_t target_str;
    target_str.data = target_buf;
    target_str.length = 0;

    const char * ver_str = NULL;
    int add_res1 = 0;
    int add_res2 = 0;
    int easy_res = 0;
    int extract_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res1 = cre2_set_add_simple(set, "foo");
    add_res2 = cre2_set_add_simple(set, "bar");
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    ver_str = cre2_version_string();
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, source_text_len, matches, 3);
    extract_res = cre2_extract_re(re, &text_input, &rewrite_str, &target_str);

    // step 4: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)easy_res + (long)extract_res + (long)(add_res1) + (long)(add_res2) + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL) + (long)(ver_str != NULL) + (long)matches[0].length + (long)target_str.length;
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}