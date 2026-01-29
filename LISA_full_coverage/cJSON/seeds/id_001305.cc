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
//<ID> 1305
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"meta\":{\"ver\":\"1.0\"},\"items\":[\"one\",\"two\"]}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(root, "parsed", parsed);

    // step 3: Operate
    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *ver = cJSON_Version();
    cJSON *ver_ref = cJSON_CreateStringReference(ver);
    cJSON_ReplaceItemViaPointer(items, first_item, ver_ref);
    cJSON *extra = cJSON_CreateString("added");
    cJSON_AddItemToArray(items, extra);

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