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
//<ID> 1369
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
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":true}");
    cJSON *str_item = cJSON_CreateString("greeting");
    cJSON *num_item = cJSON_CreateNumber(123.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "payload", arr);
    cJSON_AddNumberToObject(root, "topnum", 7.0);

    // step 3: Operate and Validate
    const char *sval = cJSON_GetStringValue(str_item);
    double nval = cJSON_GetNumberValue(num_item);
    cJSON *topnum_item = cJSON_GetObjectItem(root, "topnum");
    double topnum_v = cJSON_GetNumberValue(topnum_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *buf = (char *)scratch;
    buf[0] = out[0];
    buf[1] = sval[0];
    buf[2] = (char)('0' + ((int)nval % 10));
    buf[3] = (char)('0' + ((int)topnum_v % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}