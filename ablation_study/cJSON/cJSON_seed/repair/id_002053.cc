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
//<ID> 2053
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "label", "initial");
    cJSON *elem = cJSON_CreateString("elem0");
    cJSON_AddItemToArray(items, elem);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 2: Configure
    char json_buf[] = " { \"key\" : \"valX\" , \"num\" : 123.0 } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *key_item = cJSON_GetObjectItem(parsed, "key");
    const char *key_str = cJSON_GetStringValue(key_item);
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "dup", dup);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = key_str[0];
    cJSON *first = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first);
    buffer[2] = first_str[0];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}