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
//<ID> 268
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
    cJSON *num1 = cJSON_CreateNumber(7.0);
    cJSON *num2 = cJSON_CreateNumber(14.0);
    cJSON *raw1 = cJSON_CreateRaw("raw_content_A");

    int *ints_buf = (int *)cJSON_malloc(sizeof(int) * 3);
    ints_buf[0] = 1;
    ints_buf[1] = 2;
    ints_buf[2] = 3;
    cJSON *int_array = cJSON_CreateIntArray(ints_buf, 3);

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, raw1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "ints", int_array);
    const char *ver = cJSON_Version();
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(ver));

    // step 3: Operate & Validate
    cJSON *replacement_raw = cJSON_CreateRaw("raw_replaced");
    cJSON_bool replaced_flag = cJSON_ReplaceItemInArray(arr, 1, replacement_raw);
    (void)replaced_flag;
    cJSON *check_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool is_raw_flag = cJSON_IsRaw(check_item);
    cJSON_AddItemToObject(root, "is_second_raw", cJSON_CreateNumber((double)is_raw_flag));
    cJSON_AddItemToArray(arr, cJSON_CreateString("appended_string"));

    // step 4: Cleanup
    cJSON_free(ints_buf);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}