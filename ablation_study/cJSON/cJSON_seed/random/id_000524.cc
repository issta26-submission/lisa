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
//<ID> 524
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *str_a = "alpha";
    const char *str_b = "beta";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item_a = (cJSON *)0;
    cJSON *item_b = (cJSON *)0;
    cJSON *flag = (cJSON *)0;
    cJSON *status_item = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    const char *status_str = (const char *)0;
    int list_size = 0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool is_flag_bool = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_a = cJSON_CreateString(str_a);
    item_b = cJSON_CreateString(str_b);
    flag = cJSON_CreateTrue();

    // step 3: Configure
    added_a = cJSON_AddItemToArray(arr, item_a);
    added_b = cJSON_AddItemToArray(arr, item_b);
    added_arr = cJSON_AddItemToObject(root, "list", arr);
    status_item = cJSON_AddStringToObject(root, "status", "ready");
    cJSON_AddItemToObject(root, "flag", flag);

    // step 4: Operate
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    is_flag_bool = cJSON_IsBool((const cJSON *const)retrieved_flag);
    retrieved_list = cJSON_GetObjectItem(root, "list");
    list_size = cJSON_GetArraySize(retrieved_list);
    status_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "status"));

    // step 5: Validate
    (void)added_a;
    (void)added_b;
    (void)added_arr;
    (void)is_flag_bool;
    (void)list_size;
    (void)status_str;
    (void)status_item;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}