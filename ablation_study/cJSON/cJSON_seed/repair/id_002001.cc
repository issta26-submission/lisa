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
//<ID> 2001
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
    cJSON *arr = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddNullToObject(root, "optional");
    cJSON_AddItemToObject(child, "greeting", str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, cJSON_CreateString("world"));
    cJSON *ref_item = cJSON_CreateString("shared");
    cJSON_AddItemReferenceToArray(arr, ref_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "list", arr);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON *extra_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "extra", extra_num);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    cJSON *first = cJSON_GetArrayItem(got_list, 0);
    double num_val = cJSON_GetNumberValue(first);
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_greeting = cJSON_GetObjectItem(got_child, "greeting");
    char *gstr = cJSON_GetStringValue(got_greeting);
    buffer[0] = (char)('0' + ((int)num_val % 10));
    buffer[1] = gstr[0];
    buffer[2] = (char)('0' + (int)cJSON_IsNull(cJSON_GetObjectItem(root, "optional")));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}