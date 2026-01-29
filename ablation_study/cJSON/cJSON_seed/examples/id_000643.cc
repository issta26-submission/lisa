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
//<ID> 643
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON *item1 = cJSON_CreateString("one");
    cJSON *item2 = cJSON_CreateString("two");
    cJSON *maybe_null = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToArray(list, item1);
    cJSON_AddItemToArray(list, item2);
    cJSON_AddItemToArray(list, maybe_null);
    cJSON_AddItemToObject(root, "list", list);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("second");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(list, item2, replacement);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(list, buffer, (int)sizeof(buffer), 1);
    cJSON *first = cJSON_GetArrayItem(list, 0);
    const char *first_value = cJSON_GetStringValue(first);
    int list_size = cJSON_GetArraySize(list);
    (void)version;
    (void)replaced;
    (void)printed;
    (void)first_value;
    (void)list_size;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}