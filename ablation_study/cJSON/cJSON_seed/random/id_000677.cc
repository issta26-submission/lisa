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
//<ID> 677
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
    cJSON *num = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    cJSON_bool added_num = 0;
    cJSON_bool added_str = 0;
    double num_value = 0.0;
    char *str_value = (char *)0;
    int nonnull_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(42.5);
    str = cJSON_CreateString("cJSON_test");

    // step 3: Configure
    added_num = cJSON_AddItemToArray(arr, num);
    added_str = cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObject(root, "payload", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "payload");
    retrieved_num = cJSON_GetArrayItem(retrieved_arr, 0);
    retrieved_str = cJSON_GetArrayItem(retrieved_arr, 1);
    num_value = cJSON_GetNumberValue(retrieved_num);
    str_value = cJSON_GetStringValue(retrieved_str);
    nonnull_count = (num_value != 0.0) + (str_value != (char *)0);

    // step 5: Validate
    (void)added_num;
    (void)added_str;
    (void)num_value;
    (void)str_value;
    (void)nonnull_count;
    (void)retrieved_arr;
    (void)retrieved_num;
    (void)retrieved_str;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}