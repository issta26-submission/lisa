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
//<ID> 1640
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *embedded = cJSON_CreateString("embedded_value");
    cJSON_AddItemToObject(meta, "embedded_name", embedded);
    cJSON_AddStringToObject(root, "root_label", "root_value");

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *item1 = cJSON_CreateString("alpha");
    cJSON *item2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_str);
    cJSON_AddItemToObject(root, "first_copy", first_copy);

    // step 4: Cleanup
    char *unfmt = cJSON_PrintUnformatted(root);
    cJSON_free(unfmt);
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}