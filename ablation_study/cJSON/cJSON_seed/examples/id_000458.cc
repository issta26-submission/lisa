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
//<ID> 458
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *array = cJSON_Parse("[10,20]");
    // step 2: Configure
    cJSON *num_extra = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(array, num_extra);
    cJSON *num_from_ver = cJSON_CreateNumber((double)strlen(version));
    cJSON_AddItemToArray(array, num_from_ver);
    // step 3: Operate & Validate
    double a0 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 0));
    double a1 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 1));
    double a2 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 2));
    double a3 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 3));
    double total = a0 + a1 + a2 + a3;
    char status[128];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;sum=%.4f", version, total);
    cJSON *status_str = cJSON_CreateString(status);
    cJSON_AddItemToArray(array, status_str);
    // step 4: Cleanup
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}