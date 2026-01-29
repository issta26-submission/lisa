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
//<ID> 1388
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);

    // step 2: Configure
    cJSON_AddNumberToObject(metrics, "temperature", 36.6);
    cJSON_AddNumberToObject(metrics, "pressure", 1013.25);

    // step 3: Operate and Validate
    cJSON *temp_item = cJSON_GetObjectItem(metrics, "temperature");
    double temp = cJSON_GetNumberValue(temp_item);
    cJSON *pres_item = cJSON_GetObjectItem(metrics, "pressure");
    double pres = cJSON_GetNumberValue(pres_item);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    buffer[0] = (char)('0' + ((int)temp % 10));
    buffer[1] = (char)('0' + ((int)pres % 10));
    buffer[2] = (char)('0' + (int)printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}