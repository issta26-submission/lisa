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
//<ID> 794
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *retrieved_array = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool is_true = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("hello");
    str2 = cJSON_CreateString("world");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, str1);
    added2 = cJSON_AddItemToArray(arr, str2);
    added_arr = cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 4: Operate
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    is_true = cJSON_IsTrue(retrieved_flag);
    retrieved_array = cJSON_GetObjectItem(root, "data");
    retrieved_str = cJSON_GetArrayItem(retrieved_array, 1);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)added1 + (int)added2 + (int)added_arr + (int)is_true + (retrieved_str != (cJSON *)0) + (printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}