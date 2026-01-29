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
//<ID> 803
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_elem = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_array = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;
    double num_val = 0.0;
    int arr_size = 0;
    int ints[3];
    ints[0] = 10;
    ints[1] = 20;
    ints[2] = 30;

    // step 2: Initialize
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    arr = cJSON_CreateIntArray((const int *)ints, 3);
    num_item = cJSON_CreateNumber(7.0);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "numbers", arr);
    cJSON_AddItemToObject(meta, "lucky", num_item);
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "meta");
    retrieved_arr = cJSON_GetObjectItem(retrieved_arr, "numbers");
    is_array = cJSON_IsArray(retrieved_arr);
    retrieved_false = cJSON_GetObjectItem(root, "flag");
    is_false = cJSON_IsFalse(retrieved_false);
    retrieved_num = cJSON_GetObjectItem(meta, "lucky");
    num_val = cJSON_GetNumberValue(retrieved_num);
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_elem = cJSON_GetArrayItem(retrieved_arr, 1);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (retrieved_arr != (cJSON *)0) + (int)is_array + (retrieved_false != (cJSON *)0) + (int)is_false + (retrieved_num != (cJSON *)0) + (num_val == 7.0) + (arr_size == 3) + (retrieved_elem != (cJSON *)0) + (printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}