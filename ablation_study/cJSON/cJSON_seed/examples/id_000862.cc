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
//<ID> 862
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi = cJSON_CreateNumber(3.14159265);
    cJSON *count = cJSON_CreateNumber(0.0);
    cJSON *label_ref = cJSON_CreateStringReference("sensor-A");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "pi", pi);
    cJSON_AddItemToObjectCS(root, "count", count);
    cJSON_AddItemToObjectCS(root, "label", label_ref);

    // step 3: Operate & Validate
    double previous = cJSON_SetNumberHelper(count, 7.0);
    cJSON *previous_count = cJSON_CreateNumber(previous);
    cJSON_AddItemToObjectCS(root, "previous_count", previous_count);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}