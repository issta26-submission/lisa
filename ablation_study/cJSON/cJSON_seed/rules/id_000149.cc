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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"count\":123,\"nested\":{\"value\":3.14}}TRAIL";
    size_t buffer_length = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, buffer_length, &parse_end, 0);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    cJSON *nested_item = cJSON_GetObjectItem(root, "nested");
    cJSON *nested_value_item = cJSON_GetObjectItem(nested_item, "value");
    double count_value = cJSON_GetNumberValue(count_item);
    double nested_value = cJSON_GetNumberValue(nested_value_item);

    // step 2: Setup / Configure
    cJSON *constructed = cJSON_CreateObject();
    cJSON_AddNumberToObject(constructed, "count", count_value);
    cJSON *constructed_nested = cJSON_AddObjectToObject(constructed, "nested");
    cJSON_AddNumberToObject(constructed_nested, "value", nested_value);

    // step 3: Operate / Validate
    cJSON_bool equal = cJSON_Compare(root, constructed, 1);
    (void)equal;
    (void)parse_end;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(constructed);

    // API sequence test completed successfully
    return 66;
}