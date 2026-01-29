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
//<ID> 331
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", "1.0");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);

    // step 2: Configure
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddStringToObject(new_meta, "version", "2.0");
    cJSON_AddStringToObject(new_meta, "status", "updated");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "meta", new_meta);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON *copy_of_second = cJSON_Duplicate(second, 1);
    cJSON_AddItemToArray(items, copy_of_second);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}