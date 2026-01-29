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
//<ID> 951
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
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, t);
    cJSON *ref_to_first = cJSON_CreateObjectReference(cJSON_GetArrayItem(items, 0));
    cJSON_AddItemToObject(root, "first_ref", ref_to_first);
    cJSON *num2 = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", num2);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 0));
    char *snapshot = cJSON_PrintUnformatted(root);
    (void)second;
    (void)first_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}