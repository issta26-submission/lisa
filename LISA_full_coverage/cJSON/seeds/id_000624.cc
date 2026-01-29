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
//<ID> 624
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"alpha\":1, \"beta\":[10,20]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *new_arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(new_arr, n1);
    cJSON_AddItemToArray(new_arr, n2);
    cJSON *beta = cJSON_GetObjectItem(root, "beta");
    cJSON_bool beta_is_array = cJSON_IsArray(beta);
    cJSON_bool new_arr_is_array = cJSON_IsArray(new_arr);

    // step 3: Operate
    cJSON_AddItemToObject(root, "new_array", new_arr);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;
    (void)beta_is_array;
    (void)new_arr_is_array;
    (void)parse_end;
    (void)buffer;

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "alpha");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}