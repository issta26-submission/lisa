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
//<ID> 455
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "[1, 2, 3]";
    const char *version = cJSON_Version();
    cJSON *array = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *new_number = cJSON_CreateNumber(42.5);
    cJSON_AddItemToArray(array, new_number);

    // step 3: Operate & Validate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 1));
    double v3 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 3));
    double total = v0 + v1 + v3;
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "cjson=%s;total=%.2f", version, total);

    // step 4: Cleanup
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}