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
//<ID> 1309
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[\"one\",\"two\"],\"meta\":{\"author\":\"tester\"}}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON *replacement_ref = cJSON_CreateStringReference("replaced_by_reference");
    const char *ver_str = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(ver_str);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddItemToObject(root, "version", version_item);

    // step 3: Operate
    cJSON_ReplaceItemViaPointer(items, second_item, replacement_ref);
    cJSON *append_str = cJSON_CreateString("appended_string");
    cJSON_AddItemToArray(items, append_str);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
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