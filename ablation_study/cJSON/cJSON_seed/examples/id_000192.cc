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
//<ID> 192
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
    cJSON_AddItemToObject(root, "data", array);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":1}");
    cJSON *plain_str = cJSON_CreateString("example_string");

    // step 2: Configure
    cJSON_AddItemToArray(array, raw_item);
    cJSON_AddItemToArray(array, plain_str);
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    char status_buf[8];
    memset(status_buf, 0, sizeof(status_buf));
    status_buf[0] = (raw_check ? '1' : '0');
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "raw_ok", status);

    // step 3: Operate & Validate
    cJSON *note = cJSON_CreateString("array_populated_with_raw_and_string");
    cJSON_AddItemToObject(root, "note", note);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}