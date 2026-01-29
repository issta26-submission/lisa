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
//<ID> 1045
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *flag = cJSON_CreateBool(1);
    cJSON *name = cJSON_CreateString("orig_name");
    cJSON_AddItemToObjectCS(root, "active", flag);
    cJSON_AddItemToObjectCS(root, "items", arr);
    cJSON_AddItemToObjectCS(root, "name", name);

    // step 2: Configure
    cJSON_SetValuestring(name, "modified_name");
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("element");
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);

    // step 3: Operate
    cJSON *is_array_flag = cJSON_CreateBool(cJSON_IsArray(arr));
    cJSON_AddItemToObjectCS(root, "items_is_array", is_array_flag);
    char *print_buf = (char *)cJSON_malloc(256);
    cJSON_PrintPreallocated(root, print_buf, 256, 1);
    char *compact = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(compact);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}