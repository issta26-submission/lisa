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
//<ID> 475
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_bool root_is_obj = cJSON_IsObject(root);

    // step 2: Configure
    cJSON *arr = cJSON_AddArrayToObject(root, "arr");
    cJSON *num = cJSON_CreateNumber(1.23);
    cJSON_AddItemToObject(root, "orig", num);
    cJSON_AddItemReferenceToArray(arr, num);
    cJSON *flag = cJSON_AddTrueToObject(root, "enabled");
    double before_set = cJSON_GetNumberValue(num);
    double set_result = cJSON_SetNumberHelper(num, 9.81);

    // step 3: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool preallocated_ok = cJSON_PrintPreallocated(root, buffer, (const int)sizeof(buffer), 0);
    char *snapshot = cJSON_PrintUnformatted(root);
    int array_count = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);

    // step 4: Validate & Cleanup
    (void)root_is_obj;
    (void)before_set;
    (void)set_result;
    (void)preallocated_ok;
    (void)array_count;
    (void)first_val;
    (void)flag;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}