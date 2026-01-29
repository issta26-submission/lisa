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
//<ID> 797
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *b_true = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_s1 = (cJSON *)0;
    cJSON *retrieved_b = (cJSON *)0;
    const char *retrieved_str = (const char *)0;
    cJSON_bool added_to_object = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added3 = 0;
    cJSON_bool is_true = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("first");
    b_true = cJSON_CreateTrue();
    s2 = cJSON_CreateString("third");

    // step 3: Configure
    added_to_object = cJSON_AddItemToObject(root, "my_array", arr);
    added1 = cJSON_AddItemToArray(arr, s1);
    added2 = cJSON_AddItemToArray(arr, b_true);
    added3 = cJSON_AddItemToArray(arr, s2);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "my_array");
    retrieved_s1 = cJSON_GetArrayItem(retrieved_arr, 0);
    retrieved_b = cJSON_GetArrayItem(retrieved_arr, 1);
    retrieved_str = cJSON_GetStringValue(retrieved_s1);
    is_true = cJSON_IsTrue(retrieved_b);

    // step 5: Validate
    validation_score = (retrieved_arr != (cJSON *)0) + (retrieved_s1 != (cJSON *)0) + (retrieved_b != (cJSON *)0) + (retrieved_str != (const char *)0) + (int)is_true + (int)added_to_object + (int)added1 + (int)added2 + (int)added3;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}