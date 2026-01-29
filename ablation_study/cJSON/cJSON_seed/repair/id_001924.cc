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
//<ID> 1924
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
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, true_item);
    cJSON *nullable = cJSON_AddNullToObject(root, "nullable");

    // step 2: Configure
    cJSON *name = cJSON_CreateString("tester");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", answer);

    // step 3: Operate and Validate
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);
    int arr_size = cJSON_GetArraySize(items);
    char *name_value = cJSON_GetStringValue(name);
    buffer[250] = (char)('0' + (arr_size % 10));
    buffer[251] = (char)('0' + (printed_ok % 10));
    buffer[252] = name_value ? name_value[0] : 'X';
    buffer[255] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}