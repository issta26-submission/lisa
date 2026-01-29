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
//<ID> 1303
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[{\"id\":\"one\"},{\"id\":\"two\"}],\"meta\":{\"version\":\"1.0\"}}";
    cJSON *parsed = cJSON_Parse(json_text);
    const char *lib_version = cJSON_Version();

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "parsed", parsed);
    cJSON *libver_ref = cJSON_CreateStringReference(lib_version);
    cJSON_AddItemToObject(root, "libver", libver_ref);

    // step 3: Operate
    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *id_item = cJSON_GetObjectItemCaseSensitive(first_item, "id");
    cJSON *replacement_id = cJSON_CreateString("updated");
    cJSON_ReplaceItemViaPointer(first_item, id_item, replacement_id);
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(parsed, "meta");
    cJSON *version_item = cJSON_GetObjectItemCaseSensitive(meta, "version");
    const char *version_str = cJSON_GetStringValue(version_item);
    cJSON *version_ref = cJSON_CreateStringReference(version_str);
    cJSON_AddItemToObject(root, "version_ref", version_ref);

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