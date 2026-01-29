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
//<ID> 1639
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
    cJSON *arr_str = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool added_str = 0;
    cJSON_bool is_str = 0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello");
    arr_str = cJSON_CreateString("world");

    // step 3: Configure
    added_str = cJSON_AddItemToObject(root, "greeting", str_item);
    num_item = cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, arr_str);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "greeting");
    is_str = cJSON_IsString(retrieved);
    array_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation_score = (int)added_str + (int)is_str + (int)(num_item != (cJSON *)0) + (int)(array_size == 1);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved;
    (void)arr_str;
    (void)str_item;
    (void)num_item;
    // API sequence test completed successfully
    return 66;
}