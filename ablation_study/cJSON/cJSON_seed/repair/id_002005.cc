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
//<ID> 2005
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *first_str = cJSON_CreateString("first");
    cJSON *child_ref = cJSON_CreateObjectReference(child);

    // step 2: Configure
    cJSON_AddItemToObject(child, "num", num_item);
    cJSON_AddItemToObject(child, "text", str_item);
    cJSON_AddItemToArray(array, first_str);
    cJSON_AddItemReferenceToArray(array, child_ref);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "array", array);
    cJSON_AddNullToObject(root, "optional");

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_num = cJSON_GetObjectItem(got_child, "num");
    double num_val = cJSON_GetNumberValue(got_num);
    cJSON *got_array = cJSON_GetObjectItem(root, "array");
    int array_size = cJSON_GetArraySize(got_array);
    cJSON *first_item = cJSON_GetArrayItem(got_array, 0);
    const char *first_str_val = cJSON_GetStringValue(first_item);
    cJSON_bool was_null = cJSON_IsNull(cJSON_GetObjectItem(root, "optional"));
    buffer[0] = (char)('0' + (array_size % 10));
    buffer[1] = (char)('0' + ((int)num_val % 10));
    buffer[2] = first_str_val && first_str_val[0] ? first_str_val[0] : 'x';
    buffer[3] = (char)('0' + ((int)was_null % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}