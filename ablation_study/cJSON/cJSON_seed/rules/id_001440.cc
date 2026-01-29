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
//<ID> 1440
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
    cJSON *parsed_key = NULL;
    cJSON *oldchild = NULL;
    cJSON *replacement = NULL;
    cJSON *raw_item = NULL;
    char *printed = NULL;
    char prebuf[256];
    cJSON_bool pre_ok = 0;
    cJSON_bool replaced = 0;
    cJSON_bool is_null = 0;
    const char json[] = "{\"key\":null, \"values\":[1,2,3]}";
    const char rawjson[] = "{\"nested\":true}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    parsed = cJSON_Parse(json);
    parsed_key = cJSON_GetObjectItemCaseSensitive(parsed, "key");
    oldchild = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObjectCS(root, "number", oldchild);
    cJSON_AddItemToObjectCS(root, "parsed", parsed);
    raw_item = cJSON_AddRawToObject(root, "raw", rawjson);
    replacement = cJSON_CreateString("forty-two");
    replaced = cJSON_ReplaceItemViaPointer(root, oldchild, replacement);
    is_null = cJSON_IsNull(parsed_key);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    memset(prebuf, 0, sizeof(prebuf));
    pre_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);
    (void)raw_item;
    (void)replaced;
    (void)is_null;
    (void)pre_ok;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}