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
//<ID> 164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *orphan_null = cJSON_CreateNull();

    // step 2: Configure
    cJSON *dup_null = cJSON_Duplicate(orphan_null, 0);
    cJSON_AddItemToObject(root, "field_null", dup_null);
    cJSON *label = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate and Validate
    cJSON *replacement_number = cJSON_CreateNumber(123.456);
    cJSON_ReplaceItemViaPointer(root, dup_null, replacement_number);
    cJSON *deep_copy = cJSON_Duplicate(root, 1);
    void *temp_buf = cJSON_malloc(64);
    memset(temp_buf, 0, 64);

    // step 4: Cleanup
    cJSON_Delete(deep_copy);
    cJSON_Delete(root);
    cJSON_free(temp_buf);
    cJSON_Delete(orphan_null);
    return 66; // API sequence test completed successfully
}