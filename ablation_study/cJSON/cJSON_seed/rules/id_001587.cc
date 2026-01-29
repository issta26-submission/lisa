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
//<ID> 1587
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *arr = NULL;
    cJSON *str0 = NULL;
    cJSON *str1 = NULL;
    cJSON *obj_in_array = NULL;
    cJSON *mapped = NULL;
    cJSON *ref_to_obj = NULL;
    cJSON *arr_item = NULL;
    cJSON *copied = NULL;
    char *out = NULL;
    const char *second_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "list", arr);
    str0 = cJSON_CreateString("first");
    str1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    obj_in_array = cJSON_CreateObject();
    cJSON_AddItemToArray(arr, obj_in_array);
    cJSON_AddNumberToObject(obj_in_array, "value", 42.0);
    cJSON_AddRawToObject(root, "raw", "{\"x\":1,\"y\":2}");
    mapped = cJSON_AddObjectToObject(root, "mapped");
    cJSON_AddStringToObject(mapped, "note", "created mapped");
    ref_to_obj = cJSON_CreateObjectReference(obj_in_array);
    cJSON_AddItemToObject(root, "ref", ref_to_obj);

    // step 3: Operate / Validate
    arr_item = cJSON_GetArrayItem(arr, 1);
    second_str = cJSON_GetStringValue(arr_item);
    copied = cJSON_CreateString(second_str);
    cJSON_AddItemToObject(mapped, "second_copy", copied);

    // step 4: Cleanup
    out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}