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
//<ID> 354
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *second_elem = (cJSON *)0;
    char *json_unformatted = (char *)0;
    const char *first_str = (const char *)0;
    double second_num = 0.0;
    int arr_size = 0;
    cJSON_bool is_arr = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("one");
    num_item = cJSON_CreateNumber(2.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "numbers", arr);
    raw_item = cJSON_AddRawToObject(root, "meta", "{\"ok\":true,\"count\":2}");

    // step 4: Operate
    is_arr = cJSON_IsArray(arr);
    arr_size = cJSON_GetArraySize(arr);
    first_elem = cJSON_GetArrayItem(arr, 0);
    second_elem = cJSON_GetArrayItem(arr, 1);
    first_str = cJSON_GetStringValue(first_elem);
    second_num = cJSON_GetNumberValue(second_elem);
    json_unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    (void)is_arr;
    (void)arr_size;
    (void)first_str;
    (void)second_num;
    (void)raw_item;
    cJSON_GetObjectItem(root, "meta");

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}