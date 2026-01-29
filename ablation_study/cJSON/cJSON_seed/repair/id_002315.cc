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
//<ID> 2315
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_AddArrayToObject(root, "items");

    // step 2: Configure
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());
    cJSON_AddItemToObject(meta, "version", cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(42.0));

    // step 3: Operate
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *items_ref = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(items_ref, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (first_str && first_str[0]) ? first_str[0] : ((json && json[0]) ? json[0] : '\0');
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}