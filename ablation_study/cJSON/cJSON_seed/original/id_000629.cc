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
//<ID> 629
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"alpha\":1, \"beta\":\"text\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToObject(root, "items", items);

    // step 3: Operate
    cJSON *items_ref = cJSON_GetObjectItem(root, "items");
    cJSON_bool is_array = cJSON_IsArray(items_ref);
    int count = cJSON_GetArraySize(items_ref);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Validate & Cleanup
    (void)is_array;
    (void)count;
    cJSON_DeleteItemFromObject(root, "beta");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}