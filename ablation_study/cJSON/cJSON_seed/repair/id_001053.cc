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
//<ID> 1053
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *status = cJSON_AddStringToObject(root, "status", "initialized");
    (void)status;

    // step 2: Configure
    cJSON *elem = cJSON_CreateObject();
    cJSON_AddStringToObject(elem, "name", "alpha");
    cJSON_AddStringToObject(elem, "type", "primary");
    cJSON_AddItemToArray(arr, elem);
    cJSON *dup_elem = cJSON_Duplicate(elem, 1);
    cJSON_AddItemToArray(arr, dup_elem);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = printed[0];
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}