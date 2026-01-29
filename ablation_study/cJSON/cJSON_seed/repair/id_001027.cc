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
//<ID> 1027
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
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 0.0);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, num2);
    int arr_size = cJSON_GetArraySize(items);
    cJSON_SetNumberHelper(count_item, (double)arr_size);
    cJSON *items_lookup = cJSON_GetObjectItem(root, "items");
    cJSON_bool is_array = cJSON_IsArray(items_lookup);
    cJSON_AddBoolToObject(root, "items_is_array", is_array);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 0);

    // step 3: Operate and Validate
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    char *pretty = cJSON_Print(root);
    cJSON_free(pretty);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}