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
//<ID> 732
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"items\": []}";
    cJSON *root = cJSON_Parse(json);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *ref_item = cJSON_CreateStringReference("ref_value");
    cJSON *normal_item = cJSON_CreateString("beta");

    // step 2: Configure
    cJSON_AddItemToArray(items, ref_item);
    cJSON_AddItemToArray(items, normal_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *reparsed = cJSON_Parse(buffer);
    cJSON *items_reparsed = cJSON_GetObjectItem(reparsed, "items");
    cJSON *first_elem = cJSON_GetArrayItem(items_reparsed, 0);
    cJSON *second_elem = cJSON_GetArrayItem(items_reparsed, 1);
    const char *first_val = cJSON_GetStringValue(first_elem);
    const char *second_val = cJSON_GetStringValue(second_elem);
    (void)first_val;
    (void)second_val;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(reparsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}