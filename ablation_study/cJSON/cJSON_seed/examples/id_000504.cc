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
//<ID> 504
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_bool added0 = cJSON_AddItemToArray(items, s0);
    cJSON_bool added1 = cJSON_AddItemToArray(items, s1);
    cJSON_AddStringToObject(root, "description", "array of two strings");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "count", 2);
    cJSON_AddItemToObject(root, "meta", meta);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON *items_from_parsed = cJSON_GetObjectItem(parsed, "items");
    const char *first_str = cJSON_GetStringValue(cJSON_GetArrayItem(items_from_parsed, 0));
    sprintf(status_buf, "first=%s;equal=%d", first_str ? first_str : "null", (int)equal);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}