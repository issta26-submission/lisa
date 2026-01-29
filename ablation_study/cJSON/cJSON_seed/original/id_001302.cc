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
//<ID> 1302
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"items\":[{\"name\":\"alpha\"},{\"name\":\"beta\"}],\"meta\":\"original\"}";
    cJSON *parsed = cJSON_Parse(json);
    const char *ver = cJSON_Version();

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "data", parsed);
    cJSON *injected = cJSON_CreateString("injected");
    cJSON_AddItemToObject(root, "injection", injected);
    cJSON *ref = cJSON_CreateStringReference("static-ref");
    cJSON_AddItemToObject(root, "reference", ref);
    cJSON *verstr = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "cjson_version", verstr);

    // step 3: Operate
    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(second, "name");
    char *name_value = cJSON_GetStringValue(name_item);
    cJSON *replacement = cJSON_CreateString(name_value);
    cJSON_ReplaceItemInArray(items, 1, replacement);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}