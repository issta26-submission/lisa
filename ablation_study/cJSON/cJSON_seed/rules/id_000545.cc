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
//<ID> 545
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *json = "{\"value\": 123.5}";
    cJSON *root = cJSON_Parse(json);
    cJSON *created_num = cJSON_CreateNumber(7.5);
    char *printed = NULL;
    double val = 0.0;
    double created_val = 0.0;
    double sum = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "created_number", created_num);
    cJSON_AddNumberToObject(root, "added_number", 55.25);

    // step 3: Operate / Validate
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    val = cJSON_GetNumberValue(value_item);
    created_val = cJSON_GetNumberValue(created_num);
    sum = val + created_val;
    cJSON_AddNumberToObject(root, "sum", sum);
    printed = cJSON_Print(root);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = (char)('0' + ((int)val % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}