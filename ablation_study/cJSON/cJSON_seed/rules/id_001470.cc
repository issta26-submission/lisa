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
//<ID> 1470
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *obj = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *inner_num = NULL;
    cJSON *got_obj = NULL;
    cJSON *got_arr_item = NULL;
    cJSON *dup_arr = NULL;
    cJSON *dup_item = NULL;
    cJSON *detached = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(3.14);
    str = cJSON_CreateString("value");
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str);
    obj = cJSON_CreateObject();
    inner_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(obj, "inner", inner_num);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "obj", obj);

    // step 3: Operate / Validate
    got_obj = cJSON_GetObjectItem(root, "obj");
    got_arr_item = cJSON_GetArrayItem(arr, 0);
    dup_arr = cJSON_Duplicate(arr, 1);
    dup_item = cJSON_GetArrayItem(dup_arr, 1);
    detached = cJSON_DetachItemViaPointer(root, obj);
    cJSON_AddItemToObject(root, "reattached", detached);
    (void)got_obj;
    (void)got_arr_item;
    (void)dup_item;

    // step 4: Cleanup
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}