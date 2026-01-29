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
//<ID> 264
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    cJSON *generated_arr = (cJSON *)0;
    cJSON *num_item0 = (cJSON *)0;
    cJSON *num_item1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    float values[3];
    cJSON_bool compare_equal = (cJSON_bool)0;
    char *printed = (char *)0;

    // step 2: Initialize
    values[0] = 1.5f;
    values[1] = 2.5f;
    values[2] = 3.5f;
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray(values, 3);

    // step 3: Configure
    cJSON_AddItemToObject(root, "floats", float_arr);
    generated_arr = cJSON_CreateArray();
    num_item0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(generated_arr, num_item0);
    num_item1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(generated_arr, num_item1);
    cJSON_AddItemToObject(root, "generated", generated_arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "floats");
    compare_equal = cJSON_Compare(detached, float_arr, (cJSON_bool)1);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    return 66;
}