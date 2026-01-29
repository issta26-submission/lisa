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
//<ID> 320
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
    cJSON *value_num = cJSON_CreateNumber(0.0);
    double pi_val = cJSON_SetNumberHelper(value_num, 3.1415926535);
    cJSON_AddItemToObject(config, "pi", value_num);
    cJSON *empty = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "empty_value", empty);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "pi_copy", cJSON_CreateNumber(pi_val));

    // step 3: Operate & Validate
    char *printed = cJSON_Print(root);
    cJSON_AddItemToObject(root, "dump", cJSON_CreateString(printed));
    cJSON_DeleteItemFromObjectCaseSensitive(root, "config");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}