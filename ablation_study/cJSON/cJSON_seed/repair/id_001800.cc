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
//<ID> 1800
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
    cJSON_AddItemToObject(first, "active", cJSON_CreateTrue());
    cJSON_AddItemToArray(items, first);

    // step 2: Configure
    cJSON *second = cJSON_Duplicate(first, 1);
    cJSON_AddItemToArray(items, second);
    cJSON_AddItemToObject(root, "description", cJSON_CreateString("example-document"));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "count", cJSON_CreateNumber(2.0));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int count = cJSON_GetArraySize(items);
    cJSON *desc_item = cJSON_GetObjectItem(root, "description");
    const char *desc_str = cJSON_GetStringValue(desc_item);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *name = cJSON_GetStringValue(cJSON_GetObjectItem(first_item, "name"));
    double value = cJSON_GetNumberValue(cJSON_GetObjectItem(first_item, "value"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = desc_str ? desc_str[0] : 'Y';
    buffer[2] = name ? name[0] : 'Z';
    buffer[3] = (char)('0' + (count % 10));
    (void)value; (void)buffer; (void)name; (void)desc_str; (void)desc_item; (void)first_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}