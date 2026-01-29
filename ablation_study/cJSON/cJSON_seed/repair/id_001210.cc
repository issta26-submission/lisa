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
//<ID> 1210
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "count", 3.0);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "dup_items", dup);
    cJSON *copy_of_n2 = cJSON_Duplicate(n2, 0);
    cJSON_AddItemToArray(dup, copy_of_n2);

    // step 3: Operate and Validate
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(detached_meta, "count"));
    double n2_val = cJSON_GetNumberValue(n2);
    double final_val = count_val + n2_val;
    cJSON_AddNumberToObject(root, "final", final_val);

    // step 4: Cleanup
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}