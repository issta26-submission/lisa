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
//<ID> 647
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *b_true = cJSON_CreateBool(1);
    cJSON *b_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(items, b_true);
    cJSON_AddItemToArray(items, b_false);

    // step 2: Configure
    cJSON *status_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "status", status_bool);
    cJSON *meta = cJSON_CreateObject();
    cJSON *meta_flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(meta, "flag", meta_flag);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    int before_count = cJSON_GetArraySize(items);
    cJSON_DeleteItemFromArray(items, 1);
    int after_count = cJSON_GetArraySize(items);
    cJSON *replacement = cJSON_CreateBool(0);
    cJSON_AddItemToArray(items, replacement);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}