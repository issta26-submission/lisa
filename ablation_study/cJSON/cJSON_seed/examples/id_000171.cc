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
//<ID> 171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *placeholder = cJSON_CreateString("old_list_placeholder");
    cJSON_AddItemToObject(root, "list", placeholder);

    // step 2: Configure
    const char *const items[] = { "one", "two", "three" };
    cJSON *new_array = cJSON_CreateStringArray(items, 3);
    cJSON_ReplaceItemInObject(root, "list", new_array);

    // step 3: Operate & Validate
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    cJSON *first_elem = cJSON_GetArrayItem(got_list, 0);
    char *first_val = cJSON_GetStringValue(first_elem);
    cJSON *first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}