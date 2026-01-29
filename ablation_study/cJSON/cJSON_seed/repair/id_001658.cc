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
//<ID> 1658
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const int nums[] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *intarr = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "ints", intarr);
    cJSON *dyn = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "dynamic", dyn);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(7);
    cJSON_AddItemToArray(dyn, n1);
    cJSON *n2 = cJSON_CreateNumber(8);
    cJSON_AddItemToArray(dyn, n2);
    cJSON *n3 = cJSON_CreateNumber(9);
    cJSON_AddItemToArray(dyn, n3);

    // step 3: Operate and Validate
    cJSON *third = cJSON_GetArrayItem(intarr, 2);
    double third_val = cJSON_GetNumberValue(third);
    cJSON_bool has_ints = cJSON_HasObjectItem(root, "ints");
    cJSON_bool has_dynamic = cJSON_HasObjectItem(root, "dynamic");
    cJSON *dyn_first = cJSON_GetArrayItem(dyn, 0);
    double dyn_first_val = cJSON_GetNumberValue(dyn_first);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = (char)('0' + ((int)third_val % 10));
    buffer[1] = has_ints ? 'Y' : 'N';
    buffer[2] = has_dynamic ? 'Y' : 'N';
    buffer[3] = (printed && printed[0]) ? printed[0] : 'X';
    (void)dyn_first_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}