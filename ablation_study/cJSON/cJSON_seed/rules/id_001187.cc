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
//<ID> 1187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *parent = NULL;
    cJSON *old_child = NULL;
    cJSON *replacement = NULL;
    cJSON *item_after = NULL;
    char *printed = NULL;
    double num_value = 0.0;
    char json[] = "{\"a\":1, \"b\": false, \"nested\": {\"key\":123}}";

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    num_value = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "a"));
    parent = cJSON_CreateObject();
    old_child = cJSON_CreateNumber(num_value);
    cJSON_AddItemToObject(parent, "num", old_child);
    replacement = cJSON_CreateTrue();
    cJSON_AddTrueToObject(parent, "explicit_true");
    cJSON_ReplaceItemViaPointer(parent, old_child, replacement);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(parent);
    item_after = cJSON_GetObjectItem(parent, "num");
    (void)item_after;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(parent);
    // API sequence test completed successfully
    return 66;
}