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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char raw_lit[] = "{\"value\":123.456,\"other\":7}";
    const char *json = raw_lit;
    size_t buffer_length = sizeof(raw_lit) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, buffer_length, &parse_end, 1);
    cJSON *constructed = cJSON_CreateObject();
    cJSON *item_parsed_value = NULL;
    cJSON *item_constructed_value = NULL;
    double parsed_value = 0.0;
    double constructed_value = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(constructed, "value", 123.456);
    cJSON_AddNumberToObject(constructed, "other", 7.0);

    // step 3: Operate / Validate
    item_parsed_value = cJSON_GetObjectItem(parsed, "value");
    parsed_value = cJSON_GetNumberValue(item_parsed_value);
    item_constructed_value = cJSON_GetObjectItem(constructed, "value");
    constructed_value = cJSON_GetNumberValue(item_constructed_value);
    equal = cJSON_Compare(parsed, constructed, 1);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(constructed);
    // API sequence test completed successfully
    return 66;
}