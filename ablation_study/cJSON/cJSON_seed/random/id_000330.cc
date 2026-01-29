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
//<ID> 330
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *items_arr = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *got_array_obj = (cJSON *)0;
    cJSON *got_elem = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    char *got_str = (char *)0;
    int arr_size = 0;
    cJSON_bool is_str = (cJSON_bool)0;
    cJSON_bool is_obj = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    items_arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items_arr);

    // step 3: Configure
    elem1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items_arr, elem1);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(items_arr, true_item);
    elem2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items_arr, elem2);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(items_arr);
    got_elem = cJSON_GetArrayItem(items_arr, 0);
    got_str = cJSON_GetStringValue(got_elem);
    is_str = cJSON_IsString(got_elem);
    got_array_obj = cJSON_GetObjectItem(root, "items");
    is_obj = cJSON_IsObject(got_array_obj);
    dup = cJSON_Duplicate(root, (cJSON_bool)1);

    // step 5: Validate
    cJSON_AddItemToArray((cJSON *)cJSON_GetObjectItem(dup, "items"), cJSON_CreateString(got_str));
    cJSON_IsString(cJSON_GetArrayItem((cJSON *)cJSON_GetObjectItem(dup, "items"), 1));
    cJSON_IsObject(dup);

    // step 6: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}