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
//<ID> 1051
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
    cJSON_AddStringToObject(root, "title", "fuzz-sequence");
    cJSON *elem1 = cJSON_CreateString("alpha");
    cJSON *elem2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, elem1);
    cJSON_AddItemToArray(items, elem2);

    // step 2: Configure
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_dup);
    cJSON_AddStringToObject(root, "status", "configured");

    // step 3: Operate
    char *snapshot = cJSON_Print(root);
    cJSON_AddStringToObject(root, "snapshot", snapshot);
    cJSON_free(snapshot);

    // step 4: Cleanup and Validate
    char *final = cJSON_Print(root);
    cJSON_free(final);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}