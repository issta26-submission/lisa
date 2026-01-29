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
//<ID> 2238
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

    // step 2: Configure
    cJSON *ver_item = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(meta, "version", ver_item);
    cJSON *name_item = cJSON_CreateString("cJSONTest");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_ver = cJSON_GetObjectItem(got_meta, "version");
    double version_value = cJSON_GetNumberValue(got_ver);
    cJSON *got_name = cJSON_GetObjectItem(got_meta, "name");
    char *name_value = cJSON_GetStringValue(got_name);
    (void)version_value;
    (void)name_value;
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}