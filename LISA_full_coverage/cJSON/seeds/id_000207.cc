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
//<ID> 207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON *str_item = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, num_item);
    cJSON_AddItemToArray(array, str_item);
    cJSON *obj_item = cJSON_CreateObject();
    cJSON *inner_str = cJSON_CreateString("inner_value");
    cJSON_AddItemToObject(obj_item, "inner", inner_str);
    cJSON_AddItemToArray(array, obj_item);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(array, str_item);
    char *buffered = cJSON_PrintBuffered(root, 256, 1);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    int size_after_detach = cJSON_GetArraySize(array);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first);
    int summary = size_after_detach + (int)first_val + (int)(detached != NULL) + root->type + array->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(buffered);
    cJSON_free(tmp);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}