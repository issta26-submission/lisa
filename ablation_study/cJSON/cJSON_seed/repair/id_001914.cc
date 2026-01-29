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
//<ID> 1914
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
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(data, "value", num1);
    cJSON *label = cJSON_CreateString("old");
    cJSON_AddItemToObject(data, "label", label);

    // step 2: Configure
    cJSON *num_replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInObject(data, "value", num_replacement);
    char *updated_label = cJSON_SetValuestring(label, "updated");

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *got_value = cJSON_GetObjectItem(data, "value");
    double got_number = cJSON_GetNumberValue(got_value);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = updated_label ? updated_label[0] : 'X';
    buffer[2] = (char)('0' + ((int)got_number % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}