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
//<ID> 110
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Pattern with two named capture groups: "foo" and "bar"
    const char *pattern = "(?P<foo>\\d+)-(?P<bar>[a-zA-Z]+)";
    int pattern_len = (int)strlen(pattern);

    // Create a compiled regexp (no special options)
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, nullptr);
    if (!re) return 1;

    // Retrieve the stored pattern string and sanity-check it
    const char *stored_pattern = cre2_pattern(re);
    if (!stored_pattern) {
        cre2_delete(re);
        return 2;
    }

    // Find the index of a named capturing group
    int foo_index = cre2_find_named_capturing_groups(re, "foo");
    // foo_index may be -1 if not found; we simply continue for testing purposes

    // Request the possible match range (min and max) for the compiled regexp
    cre2_string_t min_range;
    cre2_string_t max_range;
    min_range.data = nullptr;
    min_range.length = 0;
    max_range.data = nullptr;
    max_range.length = 0;
    // Ask for ranges up to length 100
    int pmr_ret = cre2_possible_match_range(re, &min_range, &max_range, 100);
    (void)pmr_ret; // ignore detailed return for this test

    // Use cre2_easy_match to match a sample text against the same pattern.
    // Request up to 3 slots: full match + two capture groups.
    const char *text = "123-abc";
    cre2_string_t matches[3];
    for (int i = 0; i < 3; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    int match_ret = cre2_easy_match(pattern, pattern_len, text, (int)strlen(text), matches, 3);
    // match_ret > 0 indicates a match; we don't require a specific value here.
    (void)match_ret;

    // Clean up compiled regexp
    cre2_delete(re);

    return 66;
}