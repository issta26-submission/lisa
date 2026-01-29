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
//<ID> 636
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
    cJSON *num = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(data, "value", num);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "ok", flag);

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON_AddStringToObject(root, "version", ver);
    double updated = cJSON_SetNumberHelper(num, 15.5);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(data, num, replacement);

    // step 3: Operate and Validate
    cJSON *data_obj = cJSON_GetObjectItem(root, "data");
    cJSON *value_item = cJSON_GetObjectItem(data_obj, "value");
    double current_value = cJSON_GetNumberValue(value_item);
    double new_value = current_value + updated;
    cJSON_AddNumberToObject(root, "computed", new_value);
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 0);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}