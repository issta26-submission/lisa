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
//<ID> 866
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(3.14159265);
    cJSON *label_ref = cJSON_CreateStringReference("sensor-A");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "pi", num_item);
    cJSON_AddItemToObjectCS(root, "label", label_ref);

    // step 3: Operate & Validate
    double new_value = cJSON_SetNumberHelper(num_item, 6.28318530);
    cJSON *mirror_item = cJSON_CreateNumber(new_value);
    cJSON_AddItemToObjectCS(root, "mirror_pi", mirror_item);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}