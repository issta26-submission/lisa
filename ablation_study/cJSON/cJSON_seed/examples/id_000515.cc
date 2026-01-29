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
//<ID> 515
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);

    // step 2: Configure
    cJSON *s0 = cJSON_CreateString("one");
    cJSON *s1 = cJSON_CreateString("two");
    cJSON_AddItemToArray(list, s0);
    cJSON_AddItemToArray(list, s1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "desc", "sample");
    cJSON_AddItemToObjectCS(root, "meta_cs", meta);
    cJSON *root_copy = cJSON_Duplicate(root, 1);
    cJSON_AddItemToObject(root, "root_copy", root_copy);

    // step 3: Operate & Validate
    char status[200];
    memset(status, 0, sizeof(status));
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta_cs");
    cJSON *desc_item = cJSON_GetObjectItem(parsed_meta, "desc");
    const char *desc_str = cJSON_GetStringValue(desc_item);
    sprintf(status, "ver=%s;desc=%s;equal=%d", version ? version : "unknown", desc_str ? desc_str : "null", (int)equal);
    char *parsed_snapshot = cJSON_Print(parsed);
    cJSON_free(snapshot);
    cJSON_free(parsed_snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}