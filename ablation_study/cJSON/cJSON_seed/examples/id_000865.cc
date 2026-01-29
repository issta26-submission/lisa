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
//<ID> 865
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(1.5);
    cJSON *label_ref = cJSON_CreateStringReference("example_label");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "value", num_item);
    cJSON_AddItemToObjectCS(root, "label", label_ref);

    // step 3: Operate & Validate
    cJSON_SetNumberHelper(num_item, 42.0);
    cJSON *confirmed = cJSON_CreateNumber(cJSON_GetNumberValue(num_item));
    cJSON_AddItemToObjectCS(root, "confirmed", confirmed);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}