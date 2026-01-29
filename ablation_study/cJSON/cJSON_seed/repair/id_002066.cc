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
//<ID> 2066
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", num);

    // step 2: Configure
    const char raw_json[] = "{\"rawnum\":123}";
    cJSON_AddRawToObject(root, "raw", raw_json);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *label = cJSON_CreateString("label_value");
    cJSON_AddItemToObject(meta, "label", label);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    const char *label_val = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "label"));
    buffer[0] = printed[0];
    buffer[1] = name_val[0];
    buffer[2] = label_val[0];
    buffer[3] = raw_json[1];
    buffer[4] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}