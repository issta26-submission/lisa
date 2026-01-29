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
//<ID> 413
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "[7.5]";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON *p0 = cJSON_GetArrayItem(parsed, 0);
    double parsed_val = cJSON_GetNumberValue(p0);
    double updated_val = cJSON_SetNumberHelper(n1, parsed_val + 4.5);

    // step 3: Operate
    cJSON *n3 = cJSON_CreateNumber(updated_val);
    cJSON_AddItemToArray(arr, n3);
    cJSON *pnum = cJSON_CreateNumber(parsed_val);
    cJSON_AddItemToArray(arr, pnum);

    // step 4: Validate and Cleanup
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    double v3 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 3));
    double v4 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 4));
    double total = v0 + v1 + v2 + v3 + v4;
    (void)total;
    cJSON_Delete(arr);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}