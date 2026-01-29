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
//<ID> 153
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *base_array = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON *num1 = cJSON_CreateNumber(20.0);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToArray(base_array, num0);
    cJSON_AddItemToArray(base_array, num1);
    cJSON_AddItemToArray(base_array, flag);
    cJSON *array_ref = cJSON_CreateArrayReference(base_array);

    // step 2: Configure
    cJSON_AddItemToObject(root, "base", base_array);
    cJSON_AddItemReferenceToObject(root, "alias", array_ref);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);

    // step 3: Operate and Validate
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, 256, 1);
    int size = cJSON_GetArraySize(base_array);
    cJSON *first_item = cJSON_GetArrayItem(base_array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_bool has_alias = cJSON_HasObjectItem(root, "alias");
    int summary = (int)printed + size + (int)first_val + (int)has_alias + root->type + base_array->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}