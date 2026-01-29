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
//<ID> 165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "container", container);
    cJSON_AddItemToObject(container, "items", array);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *num_ref = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemReferenceToArray(array, num_ref);
    cJSON *child_obj = cJSON_CreateObject();
    cJSON_AddItemToArray(array, child_obj);

    // step 3: Operate & Validate
    double updated1 = cJSON_SetNumberHelper(num1, 42.5);
    double updated2 = cJSON_SetNumberHelper(num2, -7.25);
    cJSON *total = cJSON_CreateNumber(updated1 + updated2);
    cJSON_AddItemToObject(root, "total", total);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}