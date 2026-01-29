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
//<ID> 1801
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
    cJSON_AddItemToObject(second, "value", cJSON_CreateNumber(7.77));
    cJSON_AddItemToArray(items, second);
    cJSON_AddItemToObject(root, "meta", cJSON_CreateObject());
    cJSON_AddNumberToObject(cJSON_GetObjectItem(root, "meta"), "count", 2.0);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *first_name = cJSON_GetStringValue(cJSON_GetObjectItem(first_item, "name"));
    double first_value = cJSON_GetNumberValue(cJSON_GetObjectItem(first_item, "value"));
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    const char *second_name = cJSON_GetStringValue(cJSON_GetObjectItem(second_item, "name"));
    double second_value = cJSON_GetNumberValue(cJSON_GetObjectItem(second_item, "value"));
    buffer[0] = printed[0];
    buffer[1] = first_name[0];
    buffer[2] = (char)('0' + ((int)first_value % 10));
    buffer[3] = second_name[0];
    buffer[4] = (char)('0' + ((int)second_value % 10));
    (void)buffer; (void)first_name; (void)second_name; (void)first_value; (void)second_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}