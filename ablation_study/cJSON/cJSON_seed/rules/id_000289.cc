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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(20.25);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    int arr_size = 0;
    double version_value = 0.0;
    cJSON_bool printed_ok = 0;
    cJSON *retrieved_items = NULL;
    cJSON *version_item = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddNumberToObject(root, "version", 3.14);
    cJSON_AddStringToObject(root, "status", "ok");

    // step 3: Operate / Validate
    retrieved_items = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(retrieved_items);
    version_item = cJSON_GetObjectItem(root, "version");
    version_value = cJSON_GetNumberValue(version_item);
    printed_ok = cJSON_PrintPreallocated(root, buffer, 512, 1);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = (char)('A' + ((int)version_value % 26));
    (void)printed_ok;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}