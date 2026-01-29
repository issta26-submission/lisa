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
//<ID> 810
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *payload = cJSON_AddObjectToObject(root, "payload");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");

    // step 2: Configure
    cJSON_AddStringToObject(meta, "app", "cJSON_test");
    cJSON_AddStringToObject(root, "type", "example");
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num_item, 3.14159);
    cJSON_AddItemToObject(payload, "pi", num_item);

    // step 3: Operate & Validate
    char *unformatted_before = cJSON_PrintUnformatted(root);
    char *formatted_before = cJSON_Print(root);
    cJSON *retrieved_pi = cJSON_GetObjectItemCaseSensitive(payload, "pi");
    double pi_val = cJSON_GetNumberValue(retrieved_pi);
    cJSON_SetNumberHelper(retrieved_pi, pi_val * 2.0);
    char *unformatted_after = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted_before);
    cJSON_free(formatted_before);
    cJSON_free(unformatted_after);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}