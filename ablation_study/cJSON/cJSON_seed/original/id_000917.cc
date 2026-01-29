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
//<ID> 917
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"menu\":{\"id\":\"file\",\"value\":\"File\",\"popup\":{\"menuitem\":[{\"value\":\"New\",\"onclick\":\"CreateNewDoc()\"}]}}}";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);
    cJSON *menu = cJSON_GetObjectItemCaseSensitive(root, "menu");
    cJSON *popup = cJSON_GetObjectItemCaseSensitive(menu, "popup");
    cJSON *menuitem = cJSON_GetObjectItemCaseSensitive(popup, "menuitem");

    // step 2: Configure
    cJSON *new_item = cJSON_CreateObject();
    cJSON_AddStringToObject(new_item, "value", "Open");
    cJSON_AddStringToObject(new_item, "onclick", "OpenDoc()");
    cJSON_AddItemToArray(menuitem, new_item);

    // step 3: Operate
    cJSON *extras = cJSON_AddArrayToObject(root, "extras");
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(extras, num);
    cJSON *detached_menu = cJSON_DetachItemFromObject(root, "menu");
    cJSON_AddItemToArray(extras, detached_menu);

    // step 4: Validate & Cleanup
    cJSON *extras_check = cJSON_GetObjectItemCaseSensitive(root, "extras");
    (void)extras_check;
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    char *prebuf = (char *)buf;
    cJSON_bool ok = cJSON_PrintPreallocated(root, prebuf, 256, 1);
    (void)ok;
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}