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
//<ID> 1078
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
    cJSON *item1 = cJSON_CreateString("first");
    cJSON *item2 = cJSON_CreateString("second");
    cJSON *flag_in_array = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToArray(arr, flag_in_array);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddStringToObject(root, "label", "example_payload");
    cJSON_AddBoolToObject(root, "valid", 1);

    // step 3: Operate & Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "data");
    int count = cJSON_GetArraySize(retrieved);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_num);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}