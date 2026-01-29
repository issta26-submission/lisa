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
//<ID> 262
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
    cJSON *floatDup = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *extraArr = (cJSON *)0;
    cJSON *numItem = (cJSON *)0;
    cJSON_bool cmp_result = (cJSON_bool)0;
    char *printed = (char *)0;
    float values[3];

    // step 2: Initialize
    values[0] = 1.5f;
    values[1] = 2.5f;
    values[2] = 3.5f;
    root = cJSON_CreateObject();
    floatArr = cJSON_CreateFloatArray(values, 3);
    floatDup = cJSON_Duplicate(floatArr, 1);

    // step 3: Configure
    cJSON_AddItemToObject(root, "floats", floatArr);
    extraArr = cJSON_CreateArray();
    numItem = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(extraArr, numItem);
    cJSON_AddItemToObject(root, "extra", extraArr);

    // step 4: Operate
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "floats");
    cmp_result = cJSON_Compare((const cJSON *const)floatDup, (const cJSON *const)detached, (cJSON_bool)1);
    cJSON_AddNumberToObject(root, "compare_equal", (double)cmp_result);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(floatDup);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}