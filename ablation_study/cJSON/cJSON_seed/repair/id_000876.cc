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
//<ID> 876
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"array\":[1.0,2.0,3.5],\"meta\":{\"value\":10.0}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *arr = cJSON_GetObjectItem(parsed, "array");
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);

    // step 2: Configure
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *value_item = cJSON_GetObjectItem(meta, "value");
    double old_value = cJSON_GetNumberValue(value_item);
    cJSON_SetNumberHelper(value_item, old_value + first_val);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    parsed = NULL;

    // step 3: Operate and Validate
    cJSON *imported = cJSON_GetObjectItem(root, "imported");
    cJSON *imported_meta = cJSON_GetObjectItem(imported, "meta");
    cJSON *imported_value = cJSON_GetObjectItem(imported_meta, "value");
    double new_value = cJSON_GetNumberValue(imported_value);
    cJSON_AddNumberToObject(root, "combined", new_value);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}