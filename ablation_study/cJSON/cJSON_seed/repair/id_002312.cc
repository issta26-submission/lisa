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
//<ID> 2312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(100.0));
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());
    cJSON_AddStringToObject(meta, "title", "dataset");
    cJSON_AddNumberToObject(meta, "count", 2.0);

    // step 3: Operate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *title_item = cJSON_GetObjectItem(got_meta, "title");
    const char *title_str = cJSON_GetStringValue(title_item);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (title_str && title_str[0]) ? title_str[0] : '\0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}