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
//<ID> 2060
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "count", num);
    cJSON_AddItemToObject(child, "name", name);

    // step 2: Configure
    const char raw_json[] = "{\"rawnum\":1234,\"flag\":true}";
    cJSON_AddRawToObject(root, "raw", raw_json);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *name_str = cJSON_GetStringValue(name);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = name_str ? name_str[0] : '\0';
    buffer[2] = raw_json[1];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}