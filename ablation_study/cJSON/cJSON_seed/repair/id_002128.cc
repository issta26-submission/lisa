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
//<ID> 2128
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
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, str1);
    cJSON *str2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, str2);
    cJSON *num = cJSON_CreateNumber(84.0);
    cJSON_AddItemToObject(root, "value", num);

    // step 2: Configure
    cJSON *status = cJSON_AddStringToObject(root, "status", "ok");
    cJSON *extra = cJSON_CreateString("extra");
    cJSON_AddItemToArray(array, extra);

    // step 3: Operate and Validate
    cJSON *value_item = cJSON_GetObjectItemCaseSensitive(root, "value");
    double value = cJSON_GetNumberValue(value_item);
    double computed = value / 2.0;
    cJSON_AddNumberToObject(root, "half", computed);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)('0' + ((int)value % 10));
    buffer[1] = status->valuestring ? status->valuestring[0] : 'x';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}