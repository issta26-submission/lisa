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
//<ID> 748
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"example\",\"items\":[1,2,3],\"meta\":{\"count\":3}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int initial_size = cJSON_GetArraySize(items);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddNullToObject(root, "nullable");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, false_item);
    cJSON_AddItemToObject(root, "items_copy", items_copy);

    // step 3: Operate
    int new_size = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "initial_size", (double)initial_size);
    cJSON_AddNumberToObject(root, "new_size", (double)new_size);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}