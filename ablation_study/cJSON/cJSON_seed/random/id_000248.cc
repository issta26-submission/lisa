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
//<ID> 248
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
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *arr_from_get = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *status_item = (cJSON *)0;
    char *printed = (char *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double sum = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "numbers");
    n0 = cJSON_CreateNumber(3.0);
    n1 = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);

    // step 3: Configure
    arr_from_get = cJSON_GetObjectItem(root, "numbers");
    elem0 = cJSON_GetArrayItem(arr_from_get, 0);
    elem1 = cJSON_GetArrayItem(arr_from_get, 1);

    // step 4: Operate
    v0 = cJSON_GetNumberValue(elem0);
    v1 = cJSON_GetNumberValue(elem1);
    sum = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", sum);
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