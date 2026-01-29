#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1121
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_colors = NULL;
    cJSON *new_colors = NULL;
    cJSON *first_elem = NULL;
    cJSON *false_item = NULL;
    char *buf = NULL;
    const char *parse_end = NULL;
    const char *json = "{\"colors\":[\"red\",\"green\"],\"flag\":false}";
    size_t len = strlen(json);

    buf = (char *)cJSON_malloc(len + 1);
    memcpy(buf, json, len);
    buf[len] = '\0';

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(buf, len, &parse_end, 1);
    old_colors = cJSON_GetObjectItem(root, "colors");
    const char *const replacement_strings[] = { "blue", "yellow", "green" };
    new_colors = cJSON_CreateStringArray(replacement_strings, 3);
    cJSON_ReplaceItemViaPointer(root, old_colors, new_colors);

    // step 3: Operate / Validate
    false_item = cJSON_CreateFalse();
    first_elem = cJSON_GetArrayItem(new_colors, 0);
    cJSON_ReplaceItemViaPointer(new_colors, first_elem, false_item);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}