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
//<ID> 1807
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
    cJSON *second = cJSON_CreateObject();
    cJSON *name1 = cJSON_CreateString("alpha");
    cJSON *name2 = cJSON_CreateString("beta");
    cJSON_AddItemToObject(first, "name", name1);
    cJSON_AddItemToObject(second, "name", name2);
    cJSON_AddItemToArray(items, first);
    cJSON_AddItemToArray(items, second);

    // step 2: Configure
    cJSON_AddNumberToObject(first, "value", 3.14);
    cJSON_AddTrueToObject(second, "active");
    cJSON_AddStringToObject(root, "mode", "production");
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *items_ref = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(items_ref, 0);
    cJSON *retrieved_name = cJSON_GetObjectItem(first_item, "name");
    const char *name_str = cJSON_GetStringValue(retrieved_name);
    double value = cJSON_GetNumberValue(cJSON_GetObjectItem(first_item, "value"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_str ? name_str[0] : 'Z';
    buffer[2] = (char)('0' + (((int)value) % 10));
    (void)buffer; (void)name_str; (void)value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}