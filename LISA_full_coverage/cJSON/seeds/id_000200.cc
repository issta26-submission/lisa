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
//<ID> 200
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
    cJSON *num_item = cJSON_CreateNumber(100.0);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *sub = cJSON_CreateObject();
    cJSON *sub_val = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(sub, "value", sub_val);
    cJSON_AddItemToObject(root, "subobject", sub);

    // step 2: Configure (detach and reattach via pointer)
    cJSON *detached = cJSON_DetachItemViaPointer(root, sub);
    cJSON *destination = cJSON_CreateObject();
    cJSON_AddItemToObject(destination, "received", detached);
    cJSON_AddItemToObject(root, "moved", destination);

    // step 3: Operate and Validate (print buffered and inspect)
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    int array_size = cJSON_GetArraySize(arr);
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON_bool first_is_number = cJSON_IsNumber(first_elem);
    int summary = array_size + (int)first_val + (int)first_is_number + root->type + arr->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}