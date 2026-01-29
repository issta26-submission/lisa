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
//<ID> 1461
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\":{\"items\":[10,20,30],\"name\":\"example\"},\"active\":true}";
    cJSON *root = cJSON_Parse(json);
    cJSON *config = cJSON_GetObjectItem(root, "config");

    // step 2: Configure
    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", dup_config);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "created_by", "tester");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *items = cJSON_GetObjectItem(config, "items");
    int count = cJSON_GetArraySize(items);
    cJSON *name_item = cJSON_GetObjectItem(config, "name");
    const char *name = cJSON_GetStringValue(name_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = out_len ? out[0] : ' ';
    scratch[1] = (char)('0' + (count % 10));
    scratch[2] = name ? name[0] : 'n';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}