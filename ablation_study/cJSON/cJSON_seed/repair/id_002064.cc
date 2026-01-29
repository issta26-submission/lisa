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
//<ID> 2064
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
    cJSON *num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi", num);
    cJSON *str = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "msg", str);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    const char raw_json[] = "{\"flag\":true,\"count\":7}";
    cJSON_AddRawToObject(root, "raw", raw_json);
    cJSON *nested_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(child, "count", nested_num);
    cJSON *nested_str = cJSON_CreateString("nested");
    cJSON_AddItemToObject(child, "label", nested_str);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *root_msg = cJSON_GetStringValue(str);
    buffer[0] = printed[0];
    buffer[1] = root_msg[0];
    buffer[2] = raw_json[1];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}