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
//<ID> 923
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *label = cJSON_CreateString("initial");
    cJSON *count = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate & Validate
    cJSON *got0 = cJSON_GetArrayItem(arr, 0);
    cJSON *got_count = cJSON_GetObjectItem(root, "count");
    cJSON_SetNumberHelper(got_count, 99.5);
    cJSON *new_label = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObject(root, "label", new_label);
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    double final_count = cJSON_GetNumberValue(got_count);
    cJSON_AddNumberToObject(root, "final_count", final_count);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}