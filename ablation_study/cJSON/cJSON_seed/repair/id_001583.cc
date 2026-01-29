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
//<ID> 1583
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
    cJSON *name = cJSON_CreateString("example_name");
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "name", name);
    cJSON_AddItemToObject(child, "maybe", maybe_null);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON_bool has_name = cJSON_HasObjectItem(child, "name");
    cJSON_bool is_null = cJSON_IsNull(maybe_null);

    // step 3: Operate and Validate
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    char *scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    scratch[0] = (char)('0' + (int)has_name);
    scratch[1] = (char)('0' + (int)is_null);
    scratch[2] = (char)('0' + (int)printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}