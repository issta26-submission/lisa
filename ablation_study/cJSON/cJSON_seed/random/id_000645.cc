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
//<ID> 645
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *extra_str = (cJSON *)0;
    const char * const inputs[2] = { "alpha", "beta" };
    const char *first_val = (const char *)0;
    const char *extra_val = (const char *)0;
    cJSON_bool is_true = 0;
    int consumed = 0;

    // step 2: Initialize
    arr = cJSON_CreateStringArray(inputs, 2);
    extra_str = cJSON_CreateString("alpha");

    // step 3: Configure
    (void)consumed;

    // step 4: Operate
    first_item = cJSON_GetArrayItem(arr, 0);
    is_true = cJSON_IsTrue(first_item);
    first_val = cJSON_GetStringValue(first_item);
    extra_val = cJSON_GetStringValue(extra_str);

    // step 5: Validate
    (void)first_item;
    (void)first_val;
    (void)extra_val;
    (void)is_true;

    // step 6: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(extra_str);

    // API sequence test completed successfully
    return 66;
}