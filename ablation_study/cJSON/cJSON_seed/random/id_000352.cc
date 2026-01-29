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
//<ID> 352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *nums = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *first_num = (cJSON *)0;
    char *printed = (char *)0;
    const char *raw_text = (const char *)0;
    double first_value = 0.0;
    int arr_size = 0;
    cJSON_bool is_arr = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    nums = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "numbers", nums);
    num1 = cJSON_CreateNumber(7.0);
    num2 = cJSON_CreateNumber(13.0);
    cJSON_AddItemToArray(nums, num1);
    cJSON_AddItemToArray(nums, num2);
    cJSON_AddRawToObject(child, "raw", "{\"alpha\": true, \"beta\": [1,2,3]}");

    // step 4: Operate
    is_arr = cJSON_IsArray(nums);
    raw_item = cJSON_GetObjectItem(child, "raw");
    raw_text = cJSON_GetStringValue(raw_item);
    printed = cJSON_PrintUnformatted(root);
    arr_size = cJSON_GetArraySize(nums);
    first_num = cJSON_GetArrayItem(nums, 0);
    first_value = cJSON_GetNumberValue(first_num);

    // step 5: Validate
    (void)is_arr;
    (void)raw_text;
    (void)printed;
    (void)arr_size;
    (void)first_value;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}