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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json[] = "{\"name\":\"original\",\"count\":42,\"nested\":{\"flag\":false}}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *new_name = cJSON_CreateString("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "name", new_name);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "nested");
    int summary = (int)count_val + (int)replaced + (int)name_str[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}