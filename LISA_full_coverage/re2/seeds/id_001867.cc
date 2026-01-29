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
//<ID> 1867
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

    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;

    cre2_string_t match_arr[3];
    memset(match_arr, 0, sizeof(match_arr));
    match_arr[0].data = text_buf;
    match_arr[0].length = 0;
    match_arr[1].data = text_buf;
    match_arr[1].length = 0;
    match_arr[2].data = text_buf;
    match_arr[2].length = 0;

    cre2_string_t text_str;
    text_str.data = text_buf;
    text_str.length = source_text_len;

    cre2_string_t rewrite;
    char rewrite_buf[32];
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memcpy(rewrite_buf, "$1-$2", 5);
    rewrite.data = rewrite_buf;
    rewrite.length = 5;

    cre2_string_t target;
    char target_buf[128];
    memset(target_buf, 0, sizeof(target_buf));
    target.data = target_buf;
    target.length = 0;

    const char *ver = NULL;
    int easy_res = 0;
    int extract_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    ver = cre2_version_string();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, "foo");
    cre2_set_add_simple(set, "bar");
    cre2_set_compile(set);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    easy_res = cre2_easy_match("foo", 3, text_buf, source_text_len, match_arr, 1);
    extract_res = cre2_extract_re(re, &text_str, &rewrite, &target);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)easy_res + (long)extract_res + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)(ver != NULL) + (long)match_arr[0].length + (long)target.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}