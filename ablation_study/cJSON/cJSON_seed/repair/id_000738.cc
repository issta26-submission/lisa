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
//<ID> 738
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"value\": 2.71828, \"meta\": {\"name\":\"alpha\"}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");

    // step 2: Configure
    cJSON *dup_value = cJSON_Duplicate(value_item, 1);
    cJSON_AddItemToObject(root, "old_value", dup_value);
    cJSON *new_num = cJSON_CreateNumber(3.1415926535);

    // step 3: Operate and Validate
    cJSON_ReplaceItemViaPointer(root, value_item, new_num);
    cJSON *current_value = cJSON_GetObjectItem(root, "value");
    double validated = cJSON_GetNumberValue(current_value);
    cJSON *validated_num = cJSON_CreateNumber(validated);
    cJSON_AddItemToObject(root, "validated_value", validated_num);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}