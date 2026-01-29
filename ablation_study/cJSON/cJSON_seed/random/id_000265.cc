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
//<ID> 265
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *floatArray = (cJSON *)0;
    cJSON *copy = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *outerArray = (cJSON *)0;
    char *printed = (char *)0;
    float numbers[3];
    int count = 3;
    cJSON_bool cmp = (cJSON_bool)0;

    // step 2: Initialize
    numbers[0] = 1.5f;
    numbers[1] = 2.5f;
    numbers[2] = 3.5f;
    floatArray = cJSON_CreateFloatArray(numbers, count);
    root = cJSON_CreateObject();

    // step 3: Configure
    copy = cJSON_Duplicate(floatArray, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "copy_floats", copy);

    // step 4: Operate
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "copy_floats");
    outerArray = cJSON_CreateArray();
    cJSON_AddItemToArray(outerArray, detached);
    cmp = cJSON_Compare(floatArray, detached, (cJSON_bool)1);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(outerArray);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(outerArray);
    cJSON_Delete(floatArray);

    // API sequence test completed successfully
    return 66;
}