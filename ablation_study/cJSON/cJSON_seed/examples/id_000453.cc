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
//<ID> 453
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *parsed = cJSON_Parse("[42,7]");
    cJSON *arr = cJSON_CreateArray();

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(arr, n2);
    cJSON *parsed_first = cJSON_CreateNumber(cJSON_GetNumberValue(cJSON_GetArrayItem(parsed, 0)));
    cJSON_AddItemToArray(arr, parsed_first);
    cJSON *ver_num = cJSON_CreateNumber((double)(unsigned char)version[0]);
    cJSON_AddItemToArray(arr, ver_num);

    // step 3: Operate & Validate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    double total = v0 + v1 + v2;
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToArray(arr, total_item);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(arr);
    // API sequence test completed successfully
    return 66;
}