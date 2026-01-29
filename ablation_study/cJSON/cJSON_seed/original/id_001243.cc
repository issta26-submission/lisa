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
//<ID> 1243
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *shared = cJSON_CreateObject();
    cJSON_AddItemToObject(shared, "owner", cJSON_CreateString("alice"));
    cJSON_AddItemToObject(shared, "version", cJSON_CreateNumber(1.0));
    cJSON_AddItemToObject(root, "shared", shared);

    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemReferenceToObject(root, "shared_ref", shared);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToArray(list, cJSON_CreateString("first"));
    cJSON_AddItemToArray(list, cJSON_CreateString("second"));

    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 16);
    memset(buf, 0, printed_len + 16);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);

    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON *shared_item = cJSON_GetObjectItem(root, "shared");
    cJSON *shared_ref_item = cJSON_GetObjectItem(root, "shared_ref");
    const char *owner_from_shared = cJSON_GetStringValue(cJSON_GetObjectItem(shared_item, "owner"));
    double version_from_shared = cJSON_GetNumberValue(cJSON_GetObjectItem(shared_ref_item, "version"));
    cJSON_bool enabled_state = cJSON_IsTrue(enabled_item);
    cJSON_AddBoolToObject(root, "enabled_checked", enabled_state);

    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    return 66;
}