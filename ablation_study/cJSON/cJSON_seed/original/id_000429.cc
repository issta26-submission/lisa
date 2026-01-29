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
//<ID> 429
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int values[4] = {10, 20, 30, 40};
    cJSON *ints = cJSON_CreateIntArray(values, 4);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *first_item = cJSON_GetArrayItem(ints, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *first_num = cJSON_CreateNumber(first_val);
    cJSON_AddItemToObject(root, "first", first_num);
    cJSON *flag = cJSON_AddTrueToObject(root, "enabled");
    cJSON *placeholder = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "to_replace", placeholder);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(first_val + 5.5);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "to_replace", replacement);
    char *snapshot_before = cJSON_PrintBuffered(root, 256, 1);

    // step 3: Operate
    cJSON *replaced_item = cJSON_GetObjectItem(root, "to_replace");
    double replaced_val = cJSON_GetNumberValue(replaced_item);
    cJSON *new_number = cJSON_CreateNumber(replaced_val * 2.0);
    cJSON_AddItemToArray(ints, new_number);
    int new_size = cJSON_GetArraySize(ints);
    cJSON *last_item = cJSON_GetArrayItem(ints, new_size - 1);
    double last_val = cJSON_GetNumberValue(last_item);
    (void)last_val;
    char *snapshot_mid = cJSON_PrintBuffered(root, 128, 0);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot_before);
    cJSON_free(snapshot_mid);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}