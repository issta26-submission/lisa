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
//<ID> 191
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToArray(array, raw_item);
    cJSON *plain = cJSON_CreateString("plain_string");
    cJSON_AddItemToArray(array, plain);

    // step 2: Configure
    char status_buf[16];
    memset(status_buf, 0, sizeof(status_buf));
    status_buf[0] = '0' + (int)cJSON_IsRaw(raw_item);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "raw_status", status);

    // step 3: Operate & Validate
    const char *status_val = cJSON_GetStringValue(status);
    cJSON *echo = cJSON_CreateString(status_val);
    cJSON_AddItemToObject(root, "echo_status", echo);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}