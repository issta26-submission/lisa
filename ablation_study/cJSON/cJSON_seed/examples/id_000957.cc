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
//<ID> 957
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON *raw = cJSON_CreateRaw("{\"x\":1}");
    cJSON *name = cJSON_CreateString("device-01");

    // step 2: Configure
    cJSON_AddItemToObject(info, "name", name);
    cJSON_AddItemToObject(info, "raw", raw);
    cJSON_AddItemToObject(info, "value", num);
    cJSON_AddItemToObject(root, "info", info);
    const char *ver = cJSON_Version();
    cJSON *verstr = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "version", verstr);

    // step 3: Operate & Validate
    cJSON *got_info = cJSON_GetObjectItem(root, "info");
    cJSON *got_value = cJSON_GetObjectItem(got_info, "value");
    double extracted = cJSON_GetNumberValue(got_value);
    cJSON *got_raw = cJSON_GetObjectItem(got_info, "raw");
    cJSON_bool raw_flag = cJSON_IsRaw(got_raw);
    cJSON_AddItemToObject(root, "extracted_value", cJSON_CreateNumber(extracted));
    cJSON_AddItemToObject(root, "raw_is_raw", cJSON_CreateBool(raw_flag));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}