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
//<ID> 2129
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

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(items, num);
    cJSON *arr_str = cJSON_CreateString("array_entry");
    cJSON_AddItemToArray(items, arr_str);
    cJSON *greeting = cJSON_AddStringToObject(root, "greeting", "hello_world");

    // step 3: Operate and Validate
    cJSON *fetched_items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(fetched_items, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "copied_value", first_val);
    cJSON *fetched_greeting = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    const char *gstr = cJSON_GetStringValue(fetched_greeting);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = gstr ? gstr[0] : '\0';
    buffer[1] = (char)('0' + ((int)first_val % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}