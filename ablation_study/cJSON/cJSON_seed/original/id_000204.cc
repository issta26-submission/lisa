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
//<ID> 204
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
    cJSON *num1 = cJSON_CreateNumber(7.0);
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, str1);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToArray(array, nested);
    cJSON *child = cJSON_CreateString("child");
    cJSON_AddItemToObject(nested, "name", child);

    // step 2: Configure
    char *buffered = cJSON_PrintBuffered(root, 256, 1);
    void *tmp_buf = cJSON_malloc(128);
    memset(tmp_buf, 0, 128);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(array, nested);
    int size_after_detach = cJSON_GetArraySize(array);
    cJSON *first_elem = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON *nested_name = cJSON_GetObjectItem(detached, "name");
    const char *name_str = cJSON_GetStringValue(nested_name);
    cJSON_bool nested_is_obj = cJSON_IsObject(detached);
    int summary = size_after_detach + (int)first_val + (int)nested_is_obj + (name_str ? (int)name_str[0] : 0) + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(buffered);
    cJSON_free(tmp_buf);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}