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
//<ID> 689
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"original\",\"value\":42}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON *value_item = cJSON_GetObjectItemCaseSensitive(root, "value");

    // step 2: Configure
    char *orig_name = cJSON_GetStringValue(name_item);
    cJSON_SetValuestring(name_item, "updated_name");
    cJSON *orig_name_copy = cJSON_CreateString(orig_name);
    cJSON_AddItemToObject(root, "original_name", orig_name_copy);
    cJSON *updated_copy = cJSON_CreateString(cJSON_GetStringValue(name_item));
    cJSON_AddItemToObject(root, "updated_name_copy", updated_copy);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    char *current_name = cJSON_GetStringValue(name_item);
    (void)current_name;
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}