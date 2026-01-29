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
//<ID> 616
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(data, n1);
    cJSON_AddItemToArray(data, n2);
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddNumberToObject(root, "to_remove", 42);

    cJSON *owner_arr = cJSON_CreateArray();
    cJSON *owned_num = cJSON_CreateNumber(9.0);
    cJSON_AddItemToArray(owner_arr, owned_num);
    cJSON_AddItemToObject(root, "owner", owner_arr);

    // step 2: Configure
    cJSON *data_ref = cJSON_GetObjectItem(root, "data");
    cJSON *first_elem = cJSON_GetArrayItem(data_ref, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON_AddItemReferenceToArray(data_ref, owned_num);

    // step 3: Operate
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObject(root, "to_remove");
    cJSON *detached_data = cJSON_DetachItemFromObjectCaseSensitive(root, "data");
    cJSON *det_first = cJSON_GetArrayItem(detached_data, 0);
    double det_first_val = cJSON_GetNumberValue(det_first);

    // step 4: Validate & Cleanup
    (void)first_val;
    (void)det_first_val;
    cJSON_free(unformatted);
    cJSON_Delete(root);
    cJSON_Delete(detached_data);
    // API sequence test completed successfully
    return 66;
}