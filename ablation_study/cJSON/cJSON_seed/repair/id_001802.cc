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
//<ID> 1802
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
    cJSON *item1 = cJSON_CreateObject();
    cJSON_AddItemToObject(item1, "name", cJSON_CreateString("alpha"));
    cJSON_AddItemToObject(item1, "status", cJSON_CreateString("ok"));
    cJSON_AddItemToArray(items, item1);
    cJSON *item2 = cJSON_CreateObject();
    cJSON_AddItemToObject(item2, "name", cJSON_CreateString("beta"));
    cJSON_AddItemToObject(item2, "value", cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(items, item2);

    // step 2: Configure
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(config, "mode", cJSON_CreateString("release"));
    cJSON_AddNumberToObject(config, "threshold", 7.5);

    // step 3: Operate and Validate
    cJSON *retrieved_config = cJSON_GetObjectItem(root, "config");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *first_name = cJSON_GetObjectItem(first_item, "name");
    const char *name_str = cJSON_GetStringValue(first_name);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_str ? name_str[0] : 'Z';
    double thr = cJSON_GetNumberValue(cJSON_GetObjectItem(retrieved_config, "threshold"));
    int size = cJSON_GetArraySize(items);
    buffer[2] = (char)('0' + (size % 10));
    buffer[3] = (char)('0' + (((int)thr) % 10));
    (void)buffer; (void)thr; (void)size; (void)name_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}