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
//<ID> 547
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
    const char *json = "{\"value\":10}";
    cJSON *root = cJSON_Parse(json);
    cJSON *tmp_num = cJSON_CreateNumber(7.25);
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double orig_value = cJSON_GetNumberValue(value_item);
    double tmp_value = cJSON_GetNumberValue(tmp_num);

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "added", orig_value + tmp_value);

    // step 3: Operate / Validate
    char *printed = cJSON_Print(root);
    buffer[0] = printed[0];
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(tmp_num);

    // API sequence test completed successfully
    return 66;
}