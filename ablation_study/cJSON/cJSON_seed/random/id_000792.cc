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
//<ID> 792
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
    cJSON *bool_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_str = 0;
    cJSON_bool added_bool = 0;
    cJSON_bool is_second_true = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, "items", arr);
    str_item = cJSON_CreateString("hello");
    bool_item = cJSON_CreateTrue();
    added_str = cJSON_AddItemToArray(arr, str_item);
    added_bool = cJSON_AddItemToArray(arr, bool_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    first = cJSON_GetArrayItem(retrieved_arr, 0);
    second = cJSON_GetArrayItem(retrieved_arr, 1);
    is_second_true = cJSON_IsTrue(second);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved_arr != (cJSON *)0) + (first != (cJSON *)0) + (second != (cJSON *)0) + (int)is_second_true + (int)added_arr + (int)added_str + (int)added_bool + (printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}