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
//<ID> 1679
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *name_str = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name_str);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);
    cJSON_AddNumberToObject(meta, "value", 123.456);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "note", "initialized");
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "child", child);
    cJSON_AddNumberToObject(child, "count", 7);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *retrieved_value = cJSON_GetObjectItem(retrieved_meta, "value");
    double value = cJSON_GetNumberValue(retrieved_value);
    cJSON *retrieved_name = cJSON_GetObjectItem(root, "name");
    const char *name = cJSON_GetStringValue(retrieved_name);
    cJSON_bool is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "enabled"));
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    int intpart = (int)value;
    buffer[1] = (char)('0' + (intpart % 10));
    buffer[2] = name ? name[0] : 'Y';
    buffer[3] = (char)('0' + (int)is_false);
    (void)buffer;
    (void)child;
    (void)retrieved_meta;
    (void)retrieved_name;
    (void)retrieved_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}