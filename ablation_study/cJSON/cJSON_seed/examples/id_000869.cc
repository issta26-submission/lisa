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
//<ID> 869
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(3.1415);
    const char *device_id = "sensor-01";
    cJSON *name_ref = cJSON_CreateStringReference(device_id);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "value", num_item);
    cJSON_AddItemToObjectCS(root, "id", name_ref);
    double updated_value = cJSON_SetNumberHelper(num_item, 3.1415 * 2.0);
    cJSON *backup_item = cJSON_CreateNumber(updated_value);
    cJSON_AddItemToObjectCS(root, "backup_value", backup_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}