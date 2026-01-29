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
//<ID> 1731
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *active = cJSON_AddBoolToObject(root, "active", 1);
    cJSON *raw = cJSON_CreateRaw("{\"rawkey\":123}");
    cJSON_AddItemToObject(meta, "rawdata", raw);
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON_AddItemToObject(meta, "name", name);

    // step 2: Configure
    char *json_text = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *name_val = cJSON_GetStringValue(name);
    buffer[0] = name_val ? name_val[0] : 'X';
    buffer[1] = (char)('0' + cJSON_IsTrue(active));

    // step 3: Operate and Validate
    cJSON *retr_name = cJSON_GetObjectItem(meta, "name");
    const char *retr_name_val = cJSON_GetStringValue(retr_name);
    buffer[2] = retr_name_val ? retr_name_val[0] : 'Z';
    buffer[3] = json_text ? json_text[0] : 'Y';
    (void)buffer;
    (void)retr_name_val;

    // step 4: Cleanup
    cJSON_free(json_text);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}