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
//<ID> 1443
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *item_a = NULL;
    cJSON *item_b = NULL;
    cJSON *parsed = NULL;
    cJSON *null_item = NULL;
    cJSON *replacement = NULL;
    cJSON *bool_from_null = NULL;
    char *printed = NULL;
    cJSON_bool was_null = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    item_a = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObjectCS(root, "a", item_a);
    item_b = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "b", item_b);
    parsed = cJSON_Parse("{\"x\":1}");
    cJSON_AddItemToObject(root, "parsed", parsed);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "n", null_item);

    // step 3: Operate / Validate
    replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemViaPointer(root, item_a, replacement);
    was_null = cJSON_IsNull(null_item);
    bool_from_null = cJSON_CreateBool(was_null);
    cJSON_AddItemToObjectCS(root, "was_null", bool_from_null);
    cJSON_AddRawToObject(root, "raw", "{\"rawkey\":true}");
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}