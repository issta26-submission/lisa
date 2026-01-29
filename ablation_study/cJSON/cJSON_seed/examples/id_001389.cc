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
//<ID> 1389
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
    cJSON *num = cJSON_CreateNumber(1.5);
    cJSON *str = cJSON_CreateString("example");
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "explicit_false", false_item);
    cJSON_AddBoolToObject(root, "auto_false", 0);
    cJSON_AddStringToObject(root, "label", "demo");

    // step 3: Operate & Validate
    cJSON_bool is_arr = cJSON_IsArray(arr);
    double val = cJSON_GetNumberValue(num);
    double updated = val + (double)is_arr;
    cJSON_SetNumberHelper(num, updated);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}