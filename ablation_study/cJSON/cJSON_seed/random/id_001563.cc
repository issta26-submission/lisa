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
//<ID> 1563
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *is_valid_ptr = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_item = (cJSON *)0;
    const char *retrieved_str = (const char *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_arr_to_obj = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, item0);
    added1 = cJSON_AddItemToArray(arr, item1);
    added_arr_to_obj = cJSON_AddItemToObject(root, "letters", arr);
    is_valid_ptr = cJSON_AddBoolToObject(root, "is_valid", 1);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "letters");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_item = cJSON_GetArrayItem(retrieved_arr, 1);
    retrieved_str = cJSON_GetStringValue(retrieved_item);

    // step 5: Validate
    validation_score = (int)added0 + (int)added1 + (int)added_arr_to_obj + (int)(is_valid_ptr != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + arr_size + (int)(retrieved_str != (const char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_str;
    (void)retrieved_item;
    (void)retrieved_arr;
    (void)is_valid_ptr;
    (void)item0;
    (void)item1;
    // API sequence test completed successfully
    return 66;
}