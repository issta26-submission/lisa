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
//<ID> 335
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToObject(root, "data", array);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "ver", "1.0");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddStringToObject(new_meta, "ver", "2.0");
    cJSON_AddStringToObject(new_meta, "updated_by", "tester");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "meta", new_meta);

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *doubled = cJSON_CreateNumber(first_val * 2.0);
    cJSON_AddItemToArray(array, doubled);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}