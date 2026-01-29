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
//<ID> 805
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON *name = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(meta, "flagTrue", true_item);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "name", name);
    cJSON *raw_added = cJSON_AddRawToObject(root, "rawBox", "{\"inner\":42}");

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON *meta_got = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *flag_got = cJSON_GetObjectItemCaseSensitive(meta_got, "flagTrue");
    cJSON_bool flag_is_true = cJSON_IsTrue(flag_got);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    cJSON_AddBoolToObject(root, "flag_verified", flag_is_true);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_bool are_equal = cJSON_Compare(arr, dup_arr, 1);
    cJSON_AddBoolToObject(root, "equal_dup", are_equal);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_arr);
    // API sequence test completed successfully
    return 66;
}