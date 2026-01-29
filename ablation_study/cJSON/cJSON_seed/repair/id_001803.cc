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
//<ID> 1803
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
    cJSON *first = cJSON_CreateObject();
    cJSON_AddItemToObject(first, "name", cJSON_CreateString("alpha"));
    cJSON_AddItemToObject(first, "status", cJSON_CreateString("active"));
    cJSON_AddItemToArray(items, first);

    // step 2: Configure
    cJSON *second = cJSON_CreateObject();
    cJSON_AddItemToObject(second, "name", cJSON_CreateString("beta"));
    cJSON_AddItemToObject(second, "status", cJSON_CreateString("inactive"));
    cJSON_AddItemToArray(items, second);
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(config, "threshold", cJSON_CreateNumber(99.0));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    int count = cJSON_GetArraySize(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *name0 = cJSON_GetStringValue(cJSON_GetObjectItem(first_item, "name"));
    const char *status0 = cJSON_GetStringValue(cJSON_GetObjectItem(first_item, "status"));
    double threshold = cJSON_GetNumberValue(cJSON_GetObjectItem(config, "threshold"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name0 ? name0[0] : 'Z';
    buffer[2] = status0 ? status0[0] : 'Y';
    buffer[3] = (char)('0' + (count % 10));
    buffer[4] = (char)('0' + (((int)threshold) % 10));
    (void)buffer; (void)printed; (void)name0; (void)status0; (void)threshold; (void)count;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}