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
//<ID> 262
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
    cJSON *raw_item = cJSON_CreateRaw("raw_payload");
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    int *int_buf = (int *)cJSON_malloc(sizeof(int) * 3);
    int_buf[0] = 7;
    int_buf[1] = 8;
    int_buf[2] = 9;
    cJSON *int_array = cJSON_CreateIntArray(int_buf, 3);
    cJSON *version_ref = cJSON_CreateStringReference(cJSON_Version());

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddItemToObject(root, "version", version_ref);

    // step 3: Operate & Validate
    cJSON_bool raw_was = cJSON_IsRaw(raw_item);
    cJSON *raw_flag = cJSON_CreateBool(raw_was);
    cJSON_AddItemToObject(root, "raw_was", raw_flag);
    cJSON_ReplaceItemInArray(arr, 1, num2);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON_Delete(root_dup);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(int_buf);
    return 66;
    // API sequence test completed successfully
}