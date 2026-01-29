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
//<ID> 244
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    cJSON *status_item = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    cJSON *fetched1 = (cJSON *)0;
    char *printed = (char *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double direct = 0.0;
    double sum = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "numbers");
    cJSON_AddNumberToObject(root, "direct_number", 7.5);
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(32.5);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);

    // step 3: Configure
    fetched_arr = cJSON_GetObjectItem(root, "numbers");
    fetched0 = cJSON_GetArrayItem(fetched_arr, 0);
    fetched1 = cJSON_GetArrayItem(fetched_arr, 1);
    v0 = cJSON_GetNumberValue(fetched0);
    v1 = cJSON_GetNumberValue(fetched1);
    direct = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "direct_number"));
    sum = v0 + v1 + direct;
    sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Operate
    status_item = cJSON_CreateString("ok");
    cJSON_AddItemToObject(root, "status", status_item);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}