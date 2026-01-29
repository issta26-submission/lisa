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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"metrics\":{\"value\":3.14}, \"other\": 10}";
    size_t len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);
    cJSON *metrics = NULL;
    cJSON *value_item = NULL;
    cJSON *expected = NULL;
    double extracted_value = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    expected = cJSON_CreateObject();
    cJSON_AddNumberToObject(expected, "value", 3.14);

    // step 3: Operate / Validate
    metrics = cJSON_GetObjectItem(root, "metrics");
    value_item = cJSON_GetObjectItem(metrics, "value");
    extracted_value = cJSON_GetNumberValue(value_item);
    equal = cJSON_Compare(metrics, expected, 1);

    // step 4: Cleanup
    cJSON_Delete(expected);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}