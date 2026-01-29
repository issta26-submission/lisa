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
//<ID> 1584
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
    cJSON *name = cJSON_CreateString("example");
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "name", name);
    cJSON_AddItemToObject(child, "nullable", nullable);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON_bool has_ref = cJSON_HasObjectItem(root, "child_ref");
    cJSON *nullable_ref = cJSON_GetObjectItem(child, "nullable");
    cJSON_bool is_null = cJSON_IsNull(nullable_ref);
    cJSON *name_ref = cJSON_GetObjectItem(child, "name");
    const char *name_str = cJSON_GetStringValue(name_ref);

    // step 3: Operate and Validate
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 1);
    buf[0] = (char)('0' + (int)has_child);
    buf[1] = (char)('0' + (int)has_ref);
    buf[2] = (char)('0' + (int)printed);
    buf[3] = name_str[0];
    buf[4] = (char)('0' + (int)is_null);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}