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
//<ID> 1043
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *label = cJSON_CreateString("original_label");
    cJSON *flag = cJSON_CreateBool(0);
    cJSON_AddItemToObjectCS(root, "items", items);
    cJSON_AddItemToObjectCS(root, "label", label);
    cJSON_AddItemToObjectCS(root, "flag", flag);

    // step 2: Configure
    char *modified = cJSON_SetValuestring(label, "modified_label");

    // step 3: Operate
    cJSON_bool items_are_array = cJSON_IsArray(items);
    cJSON *array_check = cJSON_CreateBool(items_are_array);
    cJSON_AddItemToObjectCS(root, "items_is_array", array_check);
    char *compact = cJSON_PrintUnformatted(root);
    char *pretty = cJSON_Print(root);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_PrintPreallocated(root, buffer, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(compact);
    cJSON_free(pretty);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}