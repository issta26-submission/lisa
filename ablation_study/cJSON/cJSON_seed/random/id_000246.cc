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
//<ID> 246
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
    cJSON *meta = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON *fetched = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    char *printed = (char *)0;
    double extracted = 0.0;
    double first_val = 0.0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "values");
    meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Configure
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.5);
    n3 = cJSON_CreateNumber(-3.25);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);

    // step 4: Operate
    fetched = cJSON_GetArrayItem(arr, 1);
    extracted = cJSON_GetNumberValue(fetched);
    cJSON_AddNumberToObject(root, "second_value", extracted);
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(meta, "count", (double)arr_size);
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    fetched0 = cJSON_GetArrayItem(retrieved_arr, 0);
    first_val = cJSON_GetNumberValue(fetched0);
    cJSON_AddNumberToObject(root, "first_value", first_val);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}