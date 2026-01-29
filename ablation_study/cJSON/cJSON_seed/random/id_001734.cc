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
//<ID> 1734
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
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *config = (cJSON *)0;
    cJSON *config_value = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON_bool has_items = 0;
    cJSON_bool has_config = 0;
    cJSON_bool is_config_obj = 0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("alpha");
    str1 = cJSON_CreateString("beta");
    config = cJSON_CreateObject();
    config_value = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(config, "answer", config_value);
    cJSON_AddItemToObject(root, "config", config);

    // step 4: Operate
    has_items = cJSON_HasObjectItem(root, "items");
    has_config = cJSON_HasObjectItem(root, "config");
    config = cJSON_GetObjectItem(root, "config");
    is_config_obj = cJSON_IsObject(config);
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(retrieved_arr);

    // step 5: Validate
    validation = (int)has_items + (int)has_config + (int)is_config_obj + (int)(arr_size == 2);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)str0;
    (void)str1;
    (void)config;
    (void)config_value;
    (void)retrieved_arr;
    (void)has_items;
    (void)has_config;
    (void)is_config_obj;
    (void)arr_size;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}