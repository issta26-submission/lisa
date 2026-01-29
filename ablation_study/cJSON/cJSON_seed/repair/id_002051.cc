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
//<ID> 2051
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddStringToObject(root, "label", "original");

    // step 2: Configure
    char json_buf[] = " { \"msg\" : \"hello\" , \"num\": 42 } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *msg_item = cJSON_GetObjectItem(parsed, "msg");
    const char *msg_str = cJSON_GetStringValue(msg_item);
    cJSON *msg_copy = cJSON_CreateString(msg_str);
    cJSON_AddItemToArray(array, msg_copy);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "present", false_item);

    // step 3: Operate and Validate
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    char *printed = cJSON_PrintUnformatted(dup_root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = msg_str[0];
    buffer[2] = cJSON_GetStringValue(cJSON_GetArrayItem(array, 0))[0];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}