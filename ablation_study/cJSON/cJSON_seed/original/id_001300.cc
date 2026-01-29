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
//<ID> 1300
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"items\":[{\"name\":\"first\"},{\"name\":\"second\"}],\"meta\":{\"author\":\"tester\"}}";
    cJSON *parsed = cJSON_Parse(json);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(root, "parsed", parsed);
    const char *ver = cJSON_Version();
    cJSON *ver_ref = cJSON_CreateStringReference(ver);
    cJSON *ver_copy = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "version_ref", ver_ref);
    cJSON_AddItemToObject(root, "version_copy", ver_copy);

    // step 3: Operate
    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(first_item, "name");
    cJSON *new_name = cJSON_CreateString("renamed");
    cJSON_ReplaceItemViaPointer(first_item, name_item, new_name);

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