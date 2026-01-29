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
//<ID> 794
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
    cJSON *elem1 = cJSON_CreateObject();
    cJSON_AddNumberToObject(elem1, "id", 42.0);
    cJSON_AddStringToObject(elem1, "name", "item42");
    cJSON_AddTrueToObject(elem1, "active");
    cJSON_AddItemToArray(items, elem1);

    // step 2: Configure
    cJSON *elem2 = cJSON_CreateObject();
    cJSON_AddNumberToObject(elem2, "id", 7.0);
    cJSON_AddStringToObject(elem2, "name", "item7");
    cJSON_AddItemToArray(items, elem2);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first);
    cJSON_AddNumberToObject(root, "first_invalid", (double)first_is_invalid);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}