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
//<ID> 1732
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    char *json_unformatted = (char *)0;
    int arr_size = 0;
    int validation = 0;
    cJSON_bool has_items = 0;
    cJSON_bool is_obj = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("one");
    item1 = cJSON_CreateString("two");

    // step 3: Configure
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(root, "label", "sequence");

    // step 4: Operate
    has_items = cJSON_HasObjectItem(root, "items");
    retrieved_items = cJSON_GetObjectItem(root, "items");
    is_obj = cJSON_IsObject(retrieved_items);
    arr_size = cJSON_GetArraySize(retrieved_items);
    json_unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)has_items + (int)(!is_obj) + (int)(arr_size == 2) + (int)(json_unformatted != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)item0;
    (void)item1;
    (void)retrieved_items;
    (void)arr_size;
    (void)has_items;
    (void)is_obj;
    // API sequence test completed successfully
    return 66;
}