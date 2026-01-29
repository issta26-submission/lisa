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
//<ID> 615
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
    cJSON *shared_str = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *alias_item = (cJSON *)0;
    cJSON *added_count = (cJSON *)0;
    cJSON *added_flag = (cJSON *)0;
    const char *retrieved_str = (const char *)0;
    int arr_size = 0;
    cJSON_bool is_false = 0;
    double check_count = 0.0;
    double check_flag = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    shared_str = cJSON_CreateStringReference("shared_value");
    num_item = cJSON_CreateNumber(3.14159);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, shared_str);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemReferenceToObject(root, "alias", shared_str);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    added_count = cJSON_AddNumberToObject(root, "count", (double)arr_size);
    is_false = cJSON_IsFalse(false_item);
    added_flag = cJSON_AddNumberToObject(root, "false_flag", (double)is_false);

    // step 5: Validate
    alias_item = cJSON_GetObjectItem(root, "alias");
    retrieved_str = cJSON_GetStringValue(alias_item);
    check_count = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    check_flag = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "false_flag"));
    (void)retrieved_str;
    (void)check_count;
    (void)check_flag;
    (void)added_count;
    (void)added_flag;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}