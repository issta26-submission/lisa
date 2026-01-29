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
//<ID> 738
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
    cJSON *items = cJSON_CreateArray();
    cJSON *ref1 = cJSON_CreateStringReference("alpha");
    cJSON *ref2 = cJSON_CreateStringReference("beta");
    cJSON *ref3 = cJSON_CreateStringReference("gamma");

    // step 2: Configure
    cJSON_AddItemToArray(items, ref1);
    cJSON_AddItemToArray(items, ref2);
    cJSON_AddItemToArray(items, ref3);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("example"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *got_items = cJSON_GetObjectItem(parsed, "items");
    cJSON *first_item = cJSON_GetArrayItem(got_items, 0);
    const char *first_value = cJSON_GetStringValue(first_item);
    cJSON_bool first_is_string = cJSON_IsString(first_item);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    (void)version;
    (void)first_value;
    (void)first_is_string;

    // API sequence test completed successfully
    return 66;
}