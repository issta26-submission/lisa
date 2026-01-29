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
//<ID> 2000
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
    cJSON *name = cJSON_CreateString("node");
    cJSON_AddItemToObject(child, "name", name);
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *plain_str = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, plain_str);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddNullToObject(root, "optional");
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemReferenceToArray(arr, child_ref);

    // step 2: Configure
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());
    cJSON_SetValuestring(name, "node_updated");
    cJSON *extra_str = cJSON_CreateString("meta");
    cJSON_AddItemToObject(root, "meta", extra_str);

    // step 3: Operate and Validate
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_name = cJSON_GetObjectItem(got_child, "name");
    char *got_name_str = cJSON_GetStringValue(got_name);
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(got_items, 0);
    double first_num = cJSON_GetNumberValue(first_item);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (got_name_str && got_name_str[0]) ? got_name_str[0] : 'x';
    buffer[1] = (char)('0' + ((int)first_num % 10));
    buffer[2] = (char)('0' + ((int)cJSON_IsTrue(cJSON_GetObjectItem(root, "enabled")) % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}