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
//<ID> 1529
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *items = NULL;
    cJSON *item0 = NULL;
    cJSON *meta = NULL;
    cJSON *name = NULL;
    cJSON *ver_item = NULL;
    const char *version_str = NULL;
    const char *name_val = NULL;
    char json[] = "{ \"items\": [10, 20, 30], \"meta\": { \"name\": \"example\" } }";
    size_t len = sizeof(json) - 1;
    int arr_size = 0;
    double first_value = 0.0;
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, len);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    version_str = cJSON_Version();
    ver_item = cJSON_CreateStringReference(version_str);
    cJSON_AddItemToObject(root, "version", ver_item);

    // step 3: Operate / Validate
    items = cJSON_GetObjectItem(parsed, "items");
    arr_size = cJSON_GetArraySize(items);
    item0 = cJSON_GetArrayItem(items, 0);
    first_value = cJSON_GetNumberValue(item0);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    cJSON_AddNumberToObject(root, "first", first_value);
    meta = cJSON_GetObjectItem(parsed, "meta");
    name = cJSON_GetObjectItem(meta, "name");
    name_val = cJSON_GetStringValue(name);
    cJSON *name_ref = cJSON_CreateStringReference(name_val);
    cJSON_AddItemToObject(root, "name_ref", name_ref);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}