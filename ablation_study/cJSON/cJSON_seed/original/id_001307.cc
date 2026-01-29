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
//<ID> 1307
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"meta\":{\"version\":\"1.0\"},\"items\":[\"one\",\"two\",\"three\"]}";
    const char *lib_version = cJSON_Version();
    cJSON *parsed = cJSON_Parse(json);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(parsed, "meta");
    cJSON *version_item = cJSON_GetObjectItemCaseSensitive(meta, "version");
    cJSON *version_ref = cJSON_CreateStringReference(version_item->valuestring);
    cJSON_AddItemToObject(root, "version_ref", version_ref);
    cJSON *libver_str = cJSON_CreateString(lib_version);
    cJSON_AddItemToObject(root, "libver", libver_str);
    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON *second_ref = cJSON_CreateStringReference(second->valuestring);
    cJSON_AddItemToObject(root, "second_ref", second_ref);

    // step 3: Operate
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