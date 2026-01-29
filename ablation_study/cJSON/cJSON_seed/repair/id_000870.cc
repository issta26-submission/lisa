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
//<ID> 870
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[{\"val\":1.5},{\"val\":2.5}],\"meta\":{\"scale\":2.0}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *items_parsed = cJSON_GetObjectItem(parsed, "items");
    cJSON *item0 = cJSON_GetArrayItem(items_parsed, 0);
    cJSON *item1 = cJSON_GetArrayItem(items_parsed, 1);
    cJSON *val0 = cJSON_GetObjectItem(item0, "val");
    cJSON *val1 = cJSON_GetObjectItem(item1, "val");
    double orig0 = cJSON_GetNumberValue(val0);
    double orig1 = cJSON_GetNumberValue(val1);
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *scale_item = cJSON_GetObjectItem(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *scaled_arr = cJSON_CreateArray();
    cJSON *obj0 = cJSON_CreateObject();
    cJSON *num0 = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num0, orig0 * scale);
    cJSON_AddItemToObject(obj0, "val", num0);
    cJSON_AddItemToArray(scaled_arr, obj0);
    cJSON *obj1 = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num1, orig1 * scale);
    cJSON_AddItemToObject(obj1, "val", num1);
    cJSON_AddItemToArray(scaled_arr, obj1);
    cJSON_AddItemToObject(root, "scaled", scaled_arr);

    // step 3: Operate and Validate
    cJSON *scaled = cJSON_GetObjectItem(root, "scaled");
    cJSON *first_item = cJSON_GetArrayItem(scaled, 0);
    cJSON *first_val = cJSON_GetObjectItem(first_item, "val");
    double first_num = cJSON_GetNumberValue(first_val);
    cJSON *first_copy = cJSON_CreateNumber(first_num);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}