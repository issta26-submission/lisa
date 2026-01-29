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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *items[] = { "one", "two", "three" };
    cJSON *arr = cJSON_CreateStringArray(items, 3);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *orig = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "replace_me", orig);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObject(root, "replace_me", replacement);

    // step 3: Operate & Validate
    cJSON *got = cJSON_GetObjectItem(root, "replace_me");
    char *got_str = (char *)cJSON_GetStringValue(got);
    cJSON *copy = cJSON_CreateString(got_str);
    cJSON_AddItemToObject(root, "copy", copy);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    (void)printed_ok;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}