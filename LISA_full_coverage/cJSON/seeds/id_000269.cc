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
//<ID> 269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToArray(arr, num);
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddFalseToObject(obj, "flag");
    cJSON_AddNumberToObject(obj, "n", 7.0);
    cJSON_AddItemToObject(root, "obj", obj);

    // step 2: Configure
    cJSON *obj_ref = cJSON_CreateObjectReference(obj);
    cJSON_AddItemReferenceToArray(arr, obj_ref);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_bool first_is_number = cJSON_IsNumber(first);
    cJSON_AddNumberToObject(root, "array_size", (double)size);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    int summary = (int)size + (int)first_is_number + (int)(printed ? printed[0] : 0);
    (void)summary;

    // step 4: Cleanup
    if (printed) cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}