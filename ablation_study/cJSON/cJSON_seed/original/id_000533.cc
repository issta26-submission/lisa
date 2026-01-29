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
//<ID> 533
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *array = cJSON_CreateStringArray(strings, 3);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *count_item = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(root, "count", count_item);

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "cjson_version", ver_item);
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    double computed_val = count_val * 2.0;
    cJSON *computed_item = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_item);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(array, 1); // detach "beta"
    cJSON_AddItemToObject(root, "detached_item", detached); // reattach under root for management
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    double extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "computed"));
    (void)extracted;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}