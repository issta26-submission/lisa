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
//<ID> 1436
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddNumberToObject(config, "initial", 10.5);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);

    // step 3: Operate
    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToArray(items, dup_config);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *dup_first = cJSON_Duplicate(first_item, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(dup_first);
    (void)first_is_number;
    cJSON_AddItemToArray(items, dup_first);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Validate & Cleanup
    cJSON *ver_item = cJSON_GetObjectItem(root, "version");
    double ver = cJSON_GetNumberValue(ver_item);
    cJSON_AddNumberToObject(root, "version_double", ver * 2.0);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}