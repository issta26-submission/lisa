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
//<ID> 975
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *inner = cJSON_AddObjectToObject(root, "inner");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, n2);
    cJSON *name = cJSON_CreateString("deviceA");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *root_flag = cJSON_CreateTrue();
    cJSON_bool cs_added = cJSON_AddItemToObjectCS(root, "root_flag", root_flag);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(inner, "meta", meta);
    cJSON *meta_info = cJSON_CreateString("metadata");
    cJSON_AddItemToObject(meta, "info", meta_info);

    // step 3: Operate and Validate
    cJSON *detached_name = cJSON_DetachItemFromObject(config, "name");
    cJSON_bool moved_ok = cJSON_AddItemToObjectCS(inner, "moved_name", detached_name);
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)cs_added;
    (void)moved_ok;
    (void)printed;
    return 66;
}