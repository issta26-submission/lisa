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
//<ID> 442
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"tester\",\"value\":42,\"child\":{\"inner\":\"ok\"}}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(name_item);

    // step 2: Configure
    cJSON *maybe_null = cJSON_AddNullToObject(root, "optional");
    (void)maybe_null;
    cJSON *ref_name = cJSON_CreateStringReference(name_val);
    cJSON_AddItemToObject(root, "ref_name", ref_name);

    // step 3: Operate
    char *snapshot = cJSON_PrintBuffered(root, 128, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "value");

    // step 4: Validate & Cleanup
    cJSON *ref_retrieve = cJSON_GetObjectItem(root, "ref_name");
    const char *ref_text = cJSON_GetStringValue(ref_retrieve);
    (void)ref_text;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}