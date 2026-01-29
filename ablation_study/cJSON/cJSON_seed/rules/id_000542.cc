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
//<ID> 542
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"name\":\"Bob\",\"value\":10}";
    char buffer[128];
    memset(buffer, 0, 128);
    cJSON *root = cJSON_Parse(json);
    cJSON *created_num = cJSON_CreateNumber(99.5);
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double original_value = cJSON_GetNumberValue(value_item);
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "created_num", created_num);
    cJSON_AddNumberToObject(root, "added_via_helper", 123.456);
    cJSON *derived = cJSON_CreateNumber(original_value + 1.0);
    cJSON_AddItemToObject(root, "derived", derived);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    size_t len = printed ? strlen(printed) : 0;
    size_t copy_len = len <= 120 ? len : 120;
    memcpy(buffer, printed ? printed : "", copy_len);
    buffer[copy_len] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}