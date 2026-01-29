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
//<ID> 256
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *ints_arr = (cJSON *)0;
    cJSON *avg_num = (cJSON *)0;
    cJSON *status_str = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    int numbers[4];
    double v0 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    double v3 = 0.0;
    double sum = 0.0;
    double avg = 0.0;
    char *printed = (char *)0;

    // step 2: Initialize
    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;
    numbers[3] = 40;
    root = cJSON_CreateObject();
    ints_arr = cJSON_CreateIntArray(numbers, 4);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "ints", ints_arr);
    status_str = cJSON_AddStringToObject(root, "status", "ok");

    // step 4: Operate
    v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(ints_arr, 0));
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(ints_arr, 1));
    v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(ints_arr, 2));
    v3 = cJSON_GetNumberValue(cJSON_GetArrayItem(ints_arr, 3));
    sum = v0 + v1 + v2 + v3;
    avg = sum / 4.0;
    avg_num = cJSON_CreateNumber(avg);
    add_ok2 = cJSON_AddItemToObject(root, "average", avg_num);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}