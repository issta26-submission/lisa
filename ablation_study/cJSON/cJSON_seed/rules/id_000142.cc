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
//<ID> 142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"outer\":{\"value\":3.14},\"id\":7}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, 1);
    cJSON *constructed = NULL;
    cJSON *constructed_outer = NULL;
    cJSON *parsed_id_item = NULL;
    cJSON *parsed_outer_item = NULL;
    cJSON *parsed_outer_value_item = NULL;
    cJSON *constructed_id_item = NULL;
    cJSON *constructed_outer_value_item = NULL;
    double parsed_id_val = 0.0;
    double parsed_outer_val = 0.0;
    double constructed_id_val = 0.0;
    double constructed_outer_val = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    constructed = cJSON_CreateObject();
    constructed_outer = cJSON_CreateObject();
    cJSON_AddNumberToObject(constructed_outer, "value", 3.14);
    cJSON_AddItemToObject(constructed, "outer", constructed_outer);
    cJSON_AddNumberToObject(constructed, "id", 7.0);

    // step 3: Operate / Validate
    parsed_id_item = cJSON_GetObjectItem(parsed, "id");
    parsed_id_val = cJSON_GetNumberValue(parsed_id_item);
    parsed_outer_item = cJSON_GetObjectItem(parsed, "outer");
    parsed_outer_value_item = cJSON_GetObjectItem(parsed_outer_item, "value");
    parsed_outer_val = cJSON_GetNumberValue(parsed_outer_value_item);
    constructed_id_item = cJSON_GetObjectItem(constructed, "id");
    constructed_id_val = cJSON_GetNumberValue(constructed_id_item);
    constructed_outer_value_item = cJSON_GetObjectItem(constructed_outer, "value");
    constructed_outer_val = cJSON_GetNumberValue(constructed_outer_value_item);
    equal = cJSON_Compare(parsed, constructed, 1);

    // step 4: Cleanup
    (void)parsed_id_val;
    (void)parsed_outer_val;
    (void)constructed_id_val;
    (void)constructed_outer_val;
    (void)equal;
    cJSON_Delete(parsed);
    cJSON_Delete(constructed);

    // API sequence test completed successfully
    return 66;
}