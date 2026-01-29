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
//<ID> 1441
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *null_item = NULL;
    cJSON *replacement = NULL;
    cJSON *raw_item = NULL;
    cJSON *found = NULL;
    char *strval = NULL;
    cJSON_bool was_null = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "field", null_item);
    raw_item = cJSON_AddRawToObject(root, "raw_field", "{\"nested\":true}");

    // step 3: Operate / Validate
    was_null = cJSON_IsNull(null_item);
    replacement = cJSON_CreateString("replaced_value");
    replaced = cJSON_ReplaceItemViaPointer(root, null_item, replacement);
    found = cJSON_GetObjectItem(root, "field");
    strval = cJSON_GetStringValue(found);
    (void)was_null;
    (void)replaced;
    (void)raw_item;
    (void)strval;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}