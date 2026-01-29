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
//<ID> 1582
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "value", num);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybeNull", maybe_null);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON *maybe_null_ref = cJSON_GetObjectItem(root, "maybeNull");
    cJSON_bool is_null = cJSON_IsNull(maybe_null_ref);

    // step 3: Operate and Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    size_t out_len = strlen(buffer);
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)has_child);
    scratch[1] = (char)('0' + (int)is_null);
    scratch[2] = out_len ? buffer[0] : ' ';
    scratch[3] = (char)('0' + (int)printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}