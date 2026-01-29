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
//<ID> 989
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *values_arr = NULL;
    cJSON *first_elem = NULL;
    cJSON *false_item = NULL;
    char *unformatted = NULL;
    double first_value = 0.0;
    char mutable_json[] = "{\"device\":\"logger\",\"values\":[10,20,30],\"status\":true}";

    // step 2: Setup / Configure
    cJSON_Minify(mutable_json);
    parsed = cJSON_Parse(mutable_json);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(parsed, "enabled", false_item);

    // step 3: Operate / Validate
    values_arr = cJSON_GetObjectItem(parsed, "values");
    first_elem = cJSON_GetArrayItem(values_arr, 0);
    first_value = cJSON_GetNumberValue(first_elem);
    unformatted = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}