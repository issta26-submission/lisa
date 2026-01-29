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
//<ID> 392
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
    cJSON *child_obj = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *ref_obj = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool add_ref_result = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    child_obj = cJSON_CreateObject();

    // step 3: Configure
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddBoolToObject(child_obj, "active", 1);
    cJSON_AddItemToObject(root, "meta", child_obj);

    // step 4: Operate
    ref_obj = cJSON_CreateString("referenced_string");
    add_ref_result = cJSON_AddItemReferenceToObject(root, "alias", ref_obj);
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddBoolToObject(root, "single_item", (arr_size == 1));

    // step 5: Validate
    (void)add_ref_result;
    (void)arr_size;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}