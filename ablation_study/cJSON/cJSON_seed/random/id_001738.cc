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
//<ID> 1738
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    int arr_size = 0;
    int validation = 0;
    cJSON_bool has_items = 0;
    cJSON_bool is_root_obj = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    add_ok = cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("sample"));

    // step 4: Operate
    retrieved_items = cJSON_GetObjectItem(root, "items");
    retrieved_name = cJSON_GetObjectItem(root, "name");
    arr_size = cJSON_GetArraySize(retrieved_items);
    has_items = cJSON_HasObjectItem(root, "items");
    is_root_obj = cJSON_IsObject(root);

    // step 5: Validate
    validation = arr_size + (int)has_items + (int)is_root_obj + (int)(retrieved_name != (cJSON *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num0;
    (void)num1;
    (void)retrieved_items;
    (void)retrieved_name;
    (void)arr_size;
    (void)has_items;
    (void)is_root_obj;
    (void)add_ok;
    // API sequence test completed successfully
    return 66;
}