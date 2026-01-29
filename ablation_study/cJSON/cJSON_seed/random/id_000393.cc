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
//<ID> 393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    int arr_size = 0;
    const char *extracted = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(child, "items", arr);
    cJSON_AddItemToObject(root, "child", child);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    bool_item = cJSON_AddBoolToObject(child, "has_items", (cJSON_bool)arr_size);
    cJSON_AddItemReferenceToObject(root, "items_ref", arr);

    // step 5: Validate
    first_elem = cJSON_GetArrayItem(arr, 0);
    extracted = cJSON_GetStringValue(first_elem);
    (void)extracted;
    (void)arr_size;
    (void)bool_item;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}