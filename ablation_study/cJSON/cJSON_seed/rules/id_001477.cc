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
//<ID> 1477
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
    cJSON *first_item = NULL;
    cJSON *second_item = NULL;
    cJSON *got_arr = NULL;
    cJSON *got_obj = NULL;
    cJSON *inner = NULL;
    cJSON *detached_item = NULL;
    cJSON *dup_arr = NULL;
    const char *s = NULL;
    const char *dup_s = NULL;
    double n = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateString("first_elem"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(123.0));
    cJSON_AddItemToObject(root, "data", arr);
    obj = cJSON_CreateObject();
    cJSON_AddItemToObject(obj, "inner", cJSON_CreateString("inner_val"));
    cJSON_AddItemToObject(root, "meta", obj);

    // step 3: Operate / Validate
    got_arr = cJSON_GetObjectItem(root, "data");
    first_item = cJSON_GetArrayItem(got_arr, 0);
    second_item = cJSON_GetArrayItem(got_arr, 1);
    s = cJSON_GetStringValue(first_item);
    n = cJSON_GetNumberValue(second_item);
    got_obj = cJSON_GetObjectItem(root, "meta");
    inner = cJSON_GetObjectItem(got_obj, "inner");
    detached_item = cJSON_DetachItemViaPointer(got_obj, inner);
    cJSON_AddItemToArray(got_arr, detached_item);
    dup_arr = cJSON_Duplicate(got_arr, 1);
    dup_s = cJSON_GetStringValue(cJSON_GetArrayItem(dup_arr, 2));
    (void)s;
    (void)n;
    (void)dup_s;

    // step 4: Cleanup
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}