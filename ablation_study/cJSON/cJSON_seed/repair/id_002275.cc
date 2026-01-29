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
//<ID> 2275
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *raw_section = cJSON_CreateRaw("{\"inlined\":true}");
    cJSON_AddItemToObject(data, "raw_section", raw_section);
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(data, "pi", pi);
    cJSON *name = cJSON_CreateString("sensor-X");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    cJSON *active_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active_flag);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "version", "2.3");

    // step 3: Operate and Validate
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    cJSON *got_raw = cJSON_GetObjectItem(got_data, "raw_section");
    cJSON_bool raw_detected = cJSON_IsRaw(got_raw);
    cJSON *raw_flag_node = cJSON_CreateBool(raw_detected);
    cJSON_AddItemToObject(root, "raw_detected", raw_flag_node);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (int)raw_detected);
    buffer[1] = (char)('0' + (int)printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}