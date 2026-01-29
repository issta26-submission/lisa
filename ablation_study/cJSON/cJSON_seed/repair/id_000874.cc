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
//<ID> 874
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[10,20,30],\"meta\":{\"scale\":2.5}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *counter = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "counter", counter);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToArray(list, cJSON_CreateNumber(5.0));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(15.0));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(25.0));
    cJSON_AddItemToObject(root, "imported", parsed);
    parsed = NULL;

    // step 3: Operate and Validate
    cJSON *imported = cJSON_GetObjectItem(root, "imported");
    cJSON *items = cJSON_GetObjectItem(imported, "items");
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *meta = cJSON_GetObjectItem(imported, "meta");
    cJSON *scale_item = cJSON_GetObjectItem(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    double updated = cJSON_SetNumberHelper(counter, second_val * scale);
    cJSON_AddNumberToObject(root, "adjusted", updated);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}