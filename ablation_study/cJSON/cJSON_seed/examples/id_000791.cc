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
//<ID> 791
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("Hello");
    cJSON *audience = cJSON_CreateString("World");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "audience", audience);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);

    // step 3: Operate & Validate
    cJSON *item_cs = cJSON_GetObjectItemCaseSensitive(parsed, "greeting");
    cJSON *item_noncs = cJSON_GetObjectItem(parsed, "audience");
    char *val_cs = cJSON_GetStringValue(item_cs);
    char *val_noncs = cJSON_GetStringValue(item_noncs);
    size_t l1 = strlen(val_cs);
    size_t l2 = strlen(val_noncs);
    char *combined = (char *)cJSON_malloc(l1 + 1 + l2 + 1);
    memset(combined, 0, l1 + 1 + l2 + 1);
    memcpy(combined, val_cs, l1);
    combined[l1] = ' ';
    memcpy(combined + l1 + 1, val_noncs, l2);
    combined[l1 + 1 + l2] = '\0';
    cJSON *combined_item = cJSON_CreateString(combined);
    cJSON_AddItemToObject(parsed, "salutation", combined_item);
    char *val_sal = cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "salutation"));
    (void)val_sal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_free(combined);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}