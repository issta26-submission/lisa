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
//<ID> 1946
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(cfg, "label", label);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, false_item);

    // step 2: Configure
    cJSON_SetValuestring(label, "renamed_label");
    cJSON_bool items_is_array = cJSON_IsArray(items);
    cJSON_bool false_is_false = cJSON_IsFalse(false_item);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = items_is_array ? 'A' : 'a';
    buffer[1] = false_is_false ? 'F' : 'f';
    buffer[2] = buffer[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}