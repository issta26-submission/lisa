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
//<ID> 899
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.71);
    cJSON *str_item = cJSON_CreateString("sensor-A");
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":true}");

    // step 2: Configure
    cJSON_AddItemToArray(root, num1);
    cJSON_AddItemToArray(root, num2);
    cJSON_AddItemToArray(root, str_item);
    cJSON_AddItemToArray(root, raw_item);

    // step 3: Operate & Validate
    int initial_size = cJSON_GetArraySize(root);
    const char *sval = cJSON_GetStringValue(str_item);
    cJSON *computed = cJSON_CreateNumber((double)initial_size + (double)sval[0]);
    cJSON_AddItemToArray(root, computed);
    int final_size = cJSON_GetArraySize(root);

    // step 4: Cleanup
    (void)initial_size;
    (void)final_size;
    (void)sval;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}