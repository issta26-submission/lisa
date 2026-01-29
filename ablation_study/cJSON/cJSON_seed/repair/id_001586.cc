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
//<ID> 1586
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
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child, "answer", num);
    cJSON_AddItemToObject(child, "greeting", str);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nil", null_item);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 2: Configure
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON_bool null_check = cJSON_IsNull(null_item);

    // step 3: Operate and Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = (char)('0' + (int)has_child);
    scratch[1] = (char)('0' + (int)null_check);
    scratch[2] = (char)('0' + (int)printed);
    scratch[3] = buffer[0];

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}