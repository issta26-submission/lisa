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
//<ID> 1059
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
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "title", "ExampleList");

    // step 2: Configure
    cJSON *entry1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, entry1);
    cJSON *entry2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, entry2);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *first_dup = cJSON_Duplicate(first, 0);
    cJSON_AddItemToObject(root, "first_dup", first_dup);
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}