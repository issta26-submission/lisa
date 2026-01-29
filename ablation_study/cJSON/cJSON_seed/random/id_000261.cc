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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *floats = (cJSON *)0;
    cJSON *detached_floats = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool cmp_result = (cJSON_bool)0;
    float sample_floats[3];
    float extra1 = 1.5f;
    float extra2 = 2.5f;

    // step 2: Initialize
    sample_floats[0] = 1.1f;
    sample_floats[1] = 2.2f;
    sample_floats[2] = 3.3f;
    root = cJSON_CreateObject();
    floats = cJSON_CreateFloatArray(sample_floats, 3);

    // step 3: Configure
    cJSON_AddItemToObject(root, "floats", floats);
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber((double)extra1);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber((double)extra2);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    detached_floats = cJSON_DetachItemFromObjectCaseSensitive(root, "floats");
    cmp_result = cJSON_Compare(detached_floats, floats, (cJSON_bool)1);
    cJSON_AddBoolToObject(root, "floats_equal", cmp_result);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_floats);

    // API sequence test completed successfully
    return 66;
}