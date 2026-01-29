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
//<ID> 812
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddStringToObject(cfg, "version", "1.0.0");
    cJSON_AddItemToObject(cfg, "value", num_item);
    double updated_number = cJSON_SetNumberHelper(num_item, 42.5);

    // step 3: Operate & Validate
    char *unformatted = cJSON_PrintUnformatted(root);
    char *pretty = cJSON_Print(root);
    cJSON *retrieved = cJSON_GetObjectItem(cfg, "value");
    double retrieved_value = cJSON_GetNumberValue(retrieved);
    (void)updated_number;
    (void)retrieved_value;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(pretty);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}