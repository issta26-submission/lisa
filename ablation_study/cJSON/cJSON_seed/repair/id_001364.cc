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
//<ID> 1364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *raw = cJSON_CreateRaw("{\"rawkey\":123}");
    cJSON_AddItemToArray(arr, raw);
    cJSON *num = cJSON_CreateNumber(123.5);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "payload", arr);
    cJSON *msg = cJSON_CreateString("hello_world");
    cJSON_AddItemToObject(root, "msg", msg);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "format", "raw_and_number");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *num_item = cJSON_GetArrayItem(arr, 1);
    double number_value = cJSON_GetNumberValue(num_item);
    cJSON *msg_item = cJSON_GetObjectItem(root, "msg");
    char *msg_str = cJSON_GetStringValue(msg_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *buf = (char *)scratch;
    buf[0] = out ? out[0] : 0;
    buf[1] = msg_str ? msg_str[0] : 0;
    buf[2] = (char)('0' + ((int)number_value % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}