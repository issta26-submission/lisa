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
//<ID> 1301
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"items\":[\"one\",\"two\",\"three\"],\"meta\":{\"version\":\"1.0\"}}";
    cJSON *parsed = cJSON_Parse(json);
    const char *libver = cJSON_Version();
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(parsed, "meta");
    cJSON *version_item = cJSON_GetObjectItemCaseSensitive(meta, "version");
    const char *version_str = cJSON_GetStringValue(version_item);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *second = cJSON_GetArrayItem(items, 1);
    const char *second_str = cJSON_GetStringValue(second);
    cJSON *ref = cJSON_CreateStringReference(second_str);
    cJSON_AddItemToObject(root, "ref", ref);
    cJSON *version_copy = cJSON_CreateString(version_str);
    cJSON_AddItemToObject(root, "version_copy", version_copy);
    cJSON *ver_ref = cJSON_CreateStringReference(libver);
    cJSON_AddItemToObject(root, "lib_version", ver_ref);
    cJSON *constructed = cJSON_CreateString("constructed");
    cJSON_AddItemToObject(root, "note", constructed);

    // step 3: Operate
    cJSON *got_ref = cJSON_GetObjectItemCaseSensitive(root, "ref");
    cJSON *got_version = cJSON_GetObjectItemCaseSensitive(root, "version_copy");
    const char *got_ref_str = cJSON_GetStringValue(got_ref);
    const char *got_version_str = cJSON_GetStringValue(got_version);
    cJSON *echo = cJSON_CreateString(got_ref_str);
    cJSON_AddItemToObject(root, "echo", echo);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}