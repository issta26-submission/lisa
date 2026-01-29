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
//<ID> 2068
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
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *str_item = cJSON_CreateString("hello_world");
    cJSON_AddItemToObject(child, "greeting", str_item);
    cJSON *num_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(child, "pi", num_item);
    char raw_json[] = "{\"rawnum\":42}";
    cJSON_AddRawToObject(root, "raw", raw_json);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const cJSON *greeting_obj = cJSON_GetObjectItem(child, "greeting");
    const char *greeting_str = cJSON_GetStringValue(greeting_obj);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = greeting_str ? greeting_str[0] : '\0';
    buffer[2] = raw_json[1];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}