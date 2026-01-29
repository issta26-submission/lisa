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
//<ID> 1588
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
    cJSON_AddItemToObject(root, "child", child);
    cJSON *number_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "answer", number_item);
    cJSON *maybe_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_item);

    // step 2: Configure
    cJSON *child_ref_src = cJSON_GetObjectItem(root, "child");
    cJSON *child_ref = cJSON_CreateObjectReference(child_ref_src);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON_bool has_maybe = cJSON_HasObjectItem(root, "maybe");
    cJSON *maybe_lookup = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool is_maybe_null = cJSON_IsNull(maybe_lookup);
    double base_val = cJSON_GetNumberValue(number_item);
    cJSON *double_item = cJSON_CreateNumber(base_val * 2.0);
    cJSON_AddItemToObject(root, "double_answer", double_item);

    // step 3: Operate and Validate
    char buffer[256];
    memset(buffer, 0, 256);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, 256, 1);
    char *scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    scratch[0] = (char)('0' + (int)has_maybe);
    scratch[1] = (char)('0' + (int)is_maybe_null);
    scratch[2] = printed ? (buffer[0] ? buffer[0] : 'P') : 'F';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}