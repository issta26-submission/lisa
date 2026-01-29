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
//<ID> 1560
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
    cJSON *str_a = (cJSON *)0;
    cJSON *str_b = (cJSON *)0;
    cJSON *title = (cJSON *)0;
    cJSON *bool_node = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_title = (cJSON *)0;
    const char *title_str = (const char *)0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    cJSON_bool added_arr_to_obj = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_a = cJSON_CreateString("alpha");
    str_b = cJSON_CreateString("beta");
    title = cJSON_CreateString("collection title");

    // step 3: Configure
    added_a = cJSON_AddItemToArray(arr, str_a);
    added_b = cJSON_AddItemToArray(arr, str_b);
    added_arr_to_obj = cJSON_AddItemToObject(root, "items", arr);
    bool_node = cJSON_AddBoolToObject(root, "enabled", (cJSON_bool)1);
    cJSON_AddItemToObject(root, "title", title);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_title = cJSON_GetObjectItem(root, "title");
    title_str = cJSON_GetStringValue(retrieved_title);

    // step 5: Validate
    validation_score = (int)added_a + (int)added_b + (int)added_arr_to_obj + (int)(bool_node != (cJSON *)0) + arr_size + (int)(title_str != (const char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_arr;
    (void)retrieved_title;
    (void)title_str;
    (void)str_a;
    (void)str_b;
    (void)title;
    (void)bool_node;
    // API sequence test completed successfully
    return 66;
}