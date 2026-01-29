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
//<ID> 500
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON *label = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *replacement_label = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(root, label, replacement_label);
    cJSON *num_replacement = cJSON_CreateNumber(6.2830);
    cJSON_ReplaceItemViaPointer(root, num_item, num_replacement);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON *final_pi = cJSON_GetObjectItem(root, "pi");
    double final_value = cJSON_GetNumberValue(final_pi);
    (void)final_value;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}