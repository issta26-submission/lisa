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
//<ID> 1395
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON_AddNumberToObject(data, "value1", 42.0);
    cJSON_AddNumberToObject(data, "value2", 7.0);
    cJSON *flag = cJSON_AddBoolToObject(root, "flag", 1);

    // step 2: Configure
    cJSON *data_obj = cJSON_GetObjectItemCaseSensitive(root, "data");
    cJSON *value1_item = cJSON_GetObjectItemCaseSensitive(data_obj, "value1");
    cJSON *value2_item = cJSON_GetObjectItemCaseSensitive(data_obj, "value2");
    double value1 = cJSON_GetNumberValue(value1_item);
    double value2 = cJSON_GetNumberValue(value2_item);

    // step 3: Operate and Validate
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 0);
    char *scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    scratch[0] = buf[0];
    scratch[1] = (char)('0' + ((int)value1 % 10));
    scratch[2] = (char)('0' + ((int)value2 % 10));
    scratch[3] = (char)('0' + (int)printed);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}