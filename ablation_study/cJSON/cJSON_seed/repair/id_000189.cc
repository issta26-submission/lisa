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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(config, "answer", num);
    cJSON *raw = cJSON_AddRawToObject(config, "raw_payload", "{\"enabled\":true,\"count\":3}");
    cJSON *temp = cJSON_CreateString("to be removed");
    cJSON_AddItemToObject(config, "transient", temp);

    // step 3: Operate and Validate
    cJSON *retrieved_num = cJSON_GetObjectItem(config, "answer");
    double value = cJSON_GetNumberValue(retrieved_num);
    (void)value; // use value in a meaningful flow placeholder
    cJSON *retrieved_temp = cJSON_GetObjectItem(config, "transient");
    cJSON *detached = cJSON_DetachItemViaPointer(config, retrieved_temp);
    cJSON_Delete(detached);
    cJSON *copied_raw = cJSON_GetObjectItem(config, "raw_payload");
    char *raw_out = cJSON_PrintUnformatted(copied_raw);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(raw_out);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}