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
//<ID> 872
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *str_item = cJSON_CreateString("sensor-A");

    // step 2: Configure
    cJSON_AddItemToObject(root, "id", str_item);
    cJSON_AddItemToObject(root, "count", num_item);

    // step 3: Operate & Validate
    cJSON *got_id = cJSON_GetObjectItem(root, "id");
    cJSON *got_count = cJSON_GetObjectItem(root, "count");
    cJSON_bool id_is_invalid = cJSON_IsInvalid(got_id);
    cJSON_bool count_is_number = cJSON_IsNumber(got_count);
    char buffer[64];
    int written = snprintf(buffer, sizeof(buffer), "invalid=%d number=%d", (int)id_is_invalid, (int)count_is_number);
    if (written < 0) buffer[0] = '\0';
    cJSON *report = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "report", report);
    char *snapshot = cJSON_PrintUnformatted(root);
    char *final_snapshot = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(final_snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}