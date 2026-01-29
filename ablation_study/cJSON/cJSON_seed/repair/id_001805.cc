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
//<ID> 1805
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
    cJSON_AddItemToObject(first, "value", cJSON_CreateNumber(3.14));
    cJSON_AddItemToArray(items, first);

    // step 2: Configure
    cJSON *second = cJSON_CreateObject();
    cJSON_AddItemToObject(second, "name", cJSON_CreateString("beta"));
    cJSON_AddItemToObject(second, "value", cJSON_CreateNumber(2.71));
    cJSON_AddItemToArray(items, second);
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddNumberToObject(config, "threshold", 10.0);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *name_item = cJSON_GetObjectItem(first_item, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *threshold_item = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "config"), "threshold");
    double threshold_val = cJSON_GetNumberValue(threshold_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_str ? name_str[0] : 'Z';
    int th_int = (int)threshold_val;
    buffer[2] = (char)('0' + (th_int % 10));
    (void)buffer; (void)th_int; (void)name_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}