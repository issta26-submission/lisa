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
//<ID> 1024
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *dup_root = NULL;
    cJSON *name = NULL;
    cJSON *status = NULL;
    cJSON *values = NULL;
    cJSON *detached_status = NULL;
    cJSON *dup_status = NULL;
    cJSON *copied_status = NULL;
    char *printed = NULL;
    const char *device_name = "device42";
    cJSON_bool status_is_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateString(device_name);
    cJSON_AddItemToObject(root, "name", name);
    status = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "status", status);
    values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(2.0));

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup_root = cJSON_Duplicate(root, 1);
    detached_status = cJSON_DetachItemFromObject(root, "status");
    dup_status = cJSON_GetObjectItem(dup_root, "status");
    status_is_true = cJSON_IsTrue(dup_status);
    copied_status = cJSON_Duplicate(detached_status, 1);
    cJSON_AddItemToArray(values, copied_status);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached_status);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}