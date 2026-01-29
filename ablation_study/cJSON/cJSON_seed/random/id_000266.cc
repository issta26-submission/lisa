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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *floatArr = (cJSON *)0;
    cJSON *numsArr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON_bool added_float = (cJSON_bool)0;
    cJSON_bool added_nums = (cJSON_bool)0;
    cJSON_bool added_num0 = (cJSON_bool)0;
    cJSON_bool added_num1 = (cJSON_bool)0;
    cJSON_bool cmp = (cJSON_bool)0;
    char *printed = (char *)0;
    float values[3];

    // step 2: Initialize
    values[0] = 1.1f;
    values[1] = 2.2f;
    values[2] = 3.3f;
    root = cJSON_CreateObject();
    floatArr = cJSON_CreateFloatArray(values, 3);
    numsArr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(42.0);
    num1 = cJSON_CreateNumber(7.5);

    // step 3: Configure
    added_num0 = cJSON_AddItemToArray(numsArr, num0);
    added_num1 = cJSON_AddItemToArray(numsArr, num1);
    added_float = cJSON_AddItemToObject(root, "floatArray", floatArr);
    added_nums = cJSON_AddItemToObject(root, "nums", numsArr);

    // step 4: Operate
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "floatArray");
    added_num0 = cJSON_AddItemToArray(numsArr, detached);
    cmp = cJSON_Compare(detached, numsArr, (cJSON_bool)0);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}