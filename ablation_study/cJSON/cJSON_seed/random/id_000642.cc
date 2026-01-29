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
//<ID> 642
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
    cJSON *extra = (cJSON *)0;
    cJSON_bool is_true_item0 = 0;
    cJSON_bool is_true_extra = 0;
    int ptr_equal = 0;
    const char *strings[2];

    // step 2: Initialize
    strings[0] = "alpha";
    strings[1] = "beta";
    arr = cJSON_CreateStringArray((const char *const *)strings, 2);
    extra = cJSON_CreateString("alpha");

    // step 3: Configure
    (void)strings;

    // step 4: Operate
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    is_true_item0 = cJSON_IsTrue(item0);
    is_true_extra = cJSON_IsTrue(extra);
    ptr_equal = (item0 == extra);

    // step 5: Validate
    (void)is_true_item0;
    (void)is_true_extra;
    (void)ptr_equal;
    (void)item1;

    // step 6: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(extra);

    // API sequence test completed successfully
    return 66;
}