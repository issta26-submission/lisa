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
//<ID> 1644
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
    cJSON *detached1 = (cJSON *)0;
    cJSON *detached2 = (cJSON *)0;
    cJSON *restored = (cJSON *)0;
    char *json_text = (char *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_str = 0;
    cJSON_bool added_num = 0;
    cJSON_bool readded = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    added_arr = cJSON_AddItemToObject(root, "arr", arr);
    str_item = cJSON_CreateString("hello");
    num_item = cJSON_CreateNumber(123.0);
    added_str = cJSON_AddItemToArray(arr, str_item);
    added_num = cJSON_AddItemToArray(arr, num_item);

    // step 3: Operate
    json_text = cJSON_Print(root);
    cJSON_Minify(json_text);
    detached1 = cJSON_DetachItemViaPointer(arr, str_item);
    detached2 = cJSON_DetachItemFromArray(arr, 0);
    readded = cJSON_AddItemToArray(arr, detached1);
    cJSON_AddItemToObject(root, "restored", detached2);

    // step 4: Validate
    arr_size = cJSON_GetArraySize(arr);
    restored = cJSON_GetObjectItem(root, "restored");
    validation_score = (int)added_arr
        + (int)added_str
        + (int)added_num
        + (int)readded
        + (int)(json_text != (char *)0)
        + (int)(arr_size == 1)
        + (int)cJSON_IsNumber(restored);

    // step 5: Cleanup
    cJSON_free(json_text);
    cJSON_Delete(root);
    (void)validation_score;
    (void)restored;
    (void)str_item;
    (void)num_item;
    (void)detached1;
    (void)detached2;
    (void)added_arr;
    (void)added_str;
    (void)added_num;
    (void)readded;
    // API sequence test completed successfully
    return 66;
}