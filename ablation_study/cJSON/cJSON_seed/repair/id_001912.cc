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
//<ID> 1912
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "container", container);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(container, "value", num_item);
    cJSON *temp = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "temp", temp);

    // step 2: Configure
    char *old_temp_str = cJSON_SetValuestring(temp, "seven");
    cJSON *replacement = cJSON_CreateNumber(100.5);
    cJSON_ReplaceItemInObject(container, "value", replacement);

    // step 3: Operate and Validate
    cJSON *got_value = cJSON_GetObjectItem(container, "value");
    double got_num = cJSON_GetNumberValue(got_value);
    cJSON *got_temp = cJSON_GetObjectItem(root, "temp");
    char *temp_str = cJSON_GetStringValue(got_temp);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + ((int)got_num % 10));
    buffer[2] = temp_str ? temp_str[0] : 'X';
    buffer[3] = old_temp_str ? old_temp_str[0] : 'X';
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}