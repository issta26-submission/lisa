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
//<ID> 818
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *stats = cJSON_AddObjectToObject(root, "stats");
    cJSON_AddNumberToObject(stats, "value", 2.718);
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, num2);

    // step 2: Configure
    cJSON *statsObj = cJSON_GetObjectItemCaseSensitive(root, "stats");
    cJSON *valueItem = cJSON_GetObjectItemCaseSensitive(statsObj, "value");
    double retrieved_value = cJSON_GetNumberValue(valueItem);
    cJSON_AddNumberToObject(root, "value_copy", retrieved_value);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "stats");
    cJSON_AddItemToObject(root, "stats_backup", detached);
    int items_count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "items_count", (double)items_count);

    // step 4: Cleanup
    char *snapshot_root = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}