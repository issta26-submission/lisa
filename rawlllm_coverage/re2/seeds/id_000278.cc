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
//<ID> 278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Prepare a simple pattern and input text as fixed arrays so we can use sizeof.
    const char pattern[] = "a(b)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);

    char text_buf[] = "zabcx";
    cre2_string_t text;
    text.data = text_buf;
    text.length = (int)(sizeof(text_buf) - 1);

    // Create options and a set (so we can call cre2_set_delete later).
    cre2_options_t *opt = cre2_opt_new();
    cre2_set *set = nullptr;
    if (opt) {
        // cre2_anchor_t exists in the library; cast 0 to it here.
        set = cre2_set_new(opt, (cre2_anchor_t)0);
        // We can add the pattern to the set (best-effort; ignore return value).
        if (set) {
            cre2_set_add_simple(set, pattern);
            cre2_set_compile(set);
        }
    }

    // Compile the regular expression using the same options.
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // If compilation failed, clean up and return success code (we still exercised set APIs).
    if (!re) {
        if (set) cre2_set_delete(set);
        if (opt) cre2_opt_delete(opt);
        return 66;
    }

    // Use cre2_pattern (required) to obtain the canonical pattern string for the compiled regexp.
    const char *returned_pattern = cre2_pattern(re);
    // Determine number of capturing groups to size match arrays.
    int groups = cre2_num_capturing_groups(re);
    int nmatch = groups + 1;
    if (nmatch < 1) nmatch = 1;

    // Allocate match structures and ranges.
    cre2_string_t *matches = new cre2_string_t[nmatch];
    cre2_range_t *ranges = new cre2_range_t[nmatch];
    for (int i = 0; i < nmatch; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
        ranges[i].start = ranges[i].past = -1;
    }

    // Use cre2_find_and_consume with the returned pattern and the text.
    // Signature: int cre2_find_and_consume(const char * pattern, cre2_string_t * text, cre2_string_t * match, int nmatch)
    int found = cre2_find_and_consume(returned_pattern, &text, matches, nmatch);
    (void)found; // We don't rely on the return value for control flow here.

    // Convert the returned match strings into ranges relative to the original text.
    // Use the original text_buf as the reference text pointer.
    cre2_strings_to_ranges(text_buf, ranges, matches, nmatch);

    // Cleanup: free allocated arrays and cre2 objects.
    delete[] matches;
    delete[] ranges;
    cre2_delete(re);
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    return 66;
}