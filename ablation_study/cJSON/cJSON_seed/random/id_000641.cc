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
//<ID> 641
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *created_single = (cJSON *)0;
    cJSON *retrieved0 = (cJSON *)0;
    cJSON *retrieved1 = (cJSON *)0;
    cJSON_bool is_true0 = 0;
    cJSON_bool is_true1 = 0;
    char *val0 = (char *)0;
    char *val1 = (char *)0;
    char *val_single = (char *)0;
    int array_size = 0;
    const char *strings[2];

    // step 2: Initialize
    strings[0] = "alpha";
    strings[1] = "beta";
    arr = cJSON_CreateStringArray(strings, 2);
    created_single = cJSON_CreateString("gamma");

    // step 3: Configure
    array_size = cJSON_GetArraySize(arr);

    // step 4: Operate
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    retrieved1 = cJSON_GetArrayItem(arr, 1);
    is_true0 = cJSON_IsTrue(retrieved0);
    is_true1 = cJSON_IsTrue(retrieved1);
    val0 = cJSON_GetStringValue(retrieved0);
    val1 = cJSON_GetStringValue(retrieved1);
    val_single = cJSON_GetStringValue(created_single);

    // step 5: Validate
    (void)array_size;
    (void)is_true0;
    (void)is_true1;
    (void)val0;
    (void)val1;
    (void)val_single;
    (void)retrieved0;
    (void)retrieved1;

    // step 6: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(created_single);

    // API sequence test completed successfully
    return 66;
}