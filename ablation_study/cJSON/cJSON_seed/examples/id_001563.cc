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
//<ID> 1563
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "first", 2.5);
    cJSON *second = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(root, "second", second);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *gfirst = cJSON_GetObjectItem(root, "first");
    cJSON *gsecond = cJSON_GetObjectItem(root, "second");
    cJSON *genabled = cJSON_GetObjectItem(root, "enabled");

    // step 3: Operate & Validate
    cJSON_bool is_enabled = cJSON_IsTrue(genabled);
    double v1 = cJSON_GetNumberValue(gfirst);
    double v2 = cJSON_GetNumberValue(gsecond);
    double total = v1 + v2 + (double)is_enabled;
    cJSON *sum = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}