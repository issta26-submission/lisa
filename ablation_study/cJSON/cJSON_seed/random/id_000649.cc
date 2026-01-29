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
//<ID> 649
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *single = (cJSON *)0;
    cJSON_bool is_true0 = 0;
    cJSON_bool is_true1 = 0;
    const char *strings[2] = { "alpha", "beta" };
    const char *arr0_val = (const char *)0;
    const char *single_val = (const char *)0;
    int pointers_equal = 0;

    // step 2: Initialize
    arr = cJSON_CreateStringArray((const char *const *)strings, 2);
    single = cJSON_CreateString("alpha");

    // step 3: Configure
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);

    // step 4: Operate
    is_true0 = cJSON_IsTrue(item0);
    is_true1 = cJSON_IsTrue(item1);
    arr0_val = item0->valuestring;
    single_val = single->valuestring;
    pointers_equal = (arr0_val == single_val);

    // step 5: Validate
    (void)is_true0;
    (void)is_true1;
    (void)arr0_val;
    (void)single_val;
    (void)pointers_equal;

    // step 6: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(single);

    // API sequence test completed successfully
    return 66;
}