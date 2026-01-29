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
//<ID> 1645
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *created_str = cJSON_CreateString("created_with_create");
    cJSON_AddItemToObject(child, "created", created_str);

    // step 2: Configure
    cJSON_AddStringToObject(root, "direct_string", "added_directly");
    cJSON_AddStringToObject(child, "child_string", "child_added");

    // step 3: Operate
    cJSON *retrieved = cJSON_GetObjectItem(root, "direct_string");
    const char *retrieved_val = cJSON_GetStringValue(retrieved);
    cJSON *copied_str = cJSON_CreateString(retrieved_val);
    cJSON_AddItemToObject(child, "copied", copied_str);

    // step 4: Validate & Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);
    char *formatted = cJSON_Print(root);
    cJSON_free(formatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}