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
//<ID> 540
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    const char *json = "{\"value\":123.45,\"text\":\"hi\"}";
    cJSON *root = cJSON_Parse(json);
    cJSON *external_num = cJSON_CreateNumber(42.0);
    cJSON *helper_added = NULL;
    cJSON *value_item = NULL;
    double value_num = 0.0;
    double helper_num = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "external", external_num);
    helper_added = cJSON_AddNumberToObject(root, "helper", 3.1415);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    buffer[0] = printed[0];
    buffer[1] = printed[1];
    buffer[2] = '\0';
    value_item = cJSON_GetObjectItem(root, "value");
    value_num = cJSON_GetNumberValue(value_item);
    helper_num = cJSON_GetNumberValue(helper_added);
    buffer[3] = (char)('0' + ((int)value_num % 10));
    buffer[4] = (char)('0' + ((int)helper_num % 10));
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}